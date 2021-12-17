#include "qnob.h"

#include <ranges>

#include <QtCore/QThreadPool>
#include <QtCore/QScopeGuard>
#include <QtCore/QFileInfo>
#include <QtWidgets/QApplication>

#include <util/format.h>
#include <util/debug.h>
#include <util/disable_nested_event_loops.h>
#include <util/thread/worker_pool.h>

#include <lib/command_line/command_line_parser.h>
#include <lib/command_line/command_line_exception.h>
#include <lib/logging/logger.h>
#include <lib/logging/buffer_logger.h>
#include <lib/logging/file_logger.h>
#include <lib/keys/key_combination.h>

#include <platform/platform_initializer.h>
#include <platform/platform.h>

#include "qnob_config.h"
#include "qnob_args.h"
#include "qnob_command_line_parser.h"
#include "default_entity_pool.h"
#include "default_entity_factory_pool.h"

Qnob::Qnob(QObject* parent) :
    QObject(parent)
{
    m_bufferLogger.reset(new BufferLogger());
    m_logFile.reset(new QFile());
    m_fileLogger.reset(new FileLogger(m_logFile.get()));
}

Qnob::~Qnob() {
    Logger::installGlobalLogger(nullptr);
}

int Qnob::run(int argc, char** argv) {
    /* Highdpi implementation in Qt is a mess. Couldn't make it work =(. */
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");

    /* Please no nested event loops, we have this in our coding guidelines. */
    DisableNestedEventLoops dummy;

    /* Init logging first. Calls after this one might log! */
    Logger::installGlobalLogger(m_bufferLogger.get());

    /* Qt & platform classes don't throw, and they are used inside catch blocks.
     * So we initialize them here. */
    m_application.reset(new QApplication(argc, argv));
    QApplication::setQuitOnLastWindowClosed(false);
    QThread::currentThread()->setObjectName(lit("MainThread"));
    PlatformInitializer platformInitializer;

    /* Join all worker threads before platform is destroyed, there might be some deinitialization pending there. */
    auto cleanup = QScopeGuard([] {
        QThreadPool::globalInstance()->waitForDone();
        WorkerPool::globalInstance()->waitForDone();
    });

    try {
        QnobCommandLineParser parser;
        return run(parser.parse(m_application->arguments()));
    } catch (const CommandLineException& e) {
        QTextStream stream(stderr);
        stream << e.message() << Qt::endl;
        stream << tr("Try 'qnob --help' for more information.") << Qt::endl;
        return 1;
    } catch (const Exception& e) {
        xCritical("{}", e);
        return 1;
    }
}

int Qnob::run(const QnobArgs& args) {
    if (args.hideConsole)
        platform()->functions()->winHideConsole();

    switch (args.mode) {
    case ServiceMode:
        return runService(args);
    case VersionMode:
        return runVersion();
    case ListMode:
        return runList(args);
    case HelpMode:
    default:
        return runHelp();
    }
}

int Qnob::runHelp() {
    QTextStream stream(stdout);

    stream << tr("Usage: qnob [options]") << Qt::endl;
    stream << Qt::endl;

    size_t consoleWidth = 80;
    if (QSize consoleSize = platform()->functions()->consoleSize(); consoleSize.isValid())
        consoleWidth = std::max(consoleSize.width(), 40);

    CommandLineHelpOptions options;
    options.maxLineLength = consoleWidth;

    QnobCommandLineParser parser;
    parser.printSections(stream, options);

    return 0;
}

int Qnob::runVersion() {
    QTextStream stream(stdout);

    stream << sformat(tr("qnob v{}"), lit(QNOB_VERSION_STRING)) << Qt::endl;

    return 0;
}

int Qnob::runList(const QnobArgs& args) {
    QTextStream stream(stdout);

    if (args.list == EntitiesList) {
        DefaultEntityPool entityPool(nullptr);

        std::vector<Entity*> entities = entityPool.entities();
        std::ranges::sort(entities, std::less<>(), &Entity::id);

        stream << tr("Built-in entities:") << Qt::endl;
        for (Entity* entity : entities)
            stream << entity->id() << Qt::endl; // TODO: descriptions, print as a table.
    } else if (args.list == KeysList) {
        std::unordered_set<QKeyCombination> keys = platform()->shortcutManager()->bindableKeys();

        std::unordered_set<QKeyCombination> ignoredKeys = {
            Qt::Key_Cancel, Qt::Key_Mode_switch, Qt::Key_Select, Qt::Key_Printer, Qt::Key_Execute, Qt::Key_Sleep,
            Qt::Key_Massyo, Qt::Key_Touroku, Qt::Key_Stop, Qt::Key_Play
        };

        QStringList keyStrings;
        for (QKeyCombination key : keys)
            if (!ignoredKeys.contains(key))
                keyStrings.push_back(sformat(L"'{}'", serialized(key)));
        keyStrings.sort(Qt::CaseInsensitive);

        stream << tr("Available keys:") << Qt::endl;
        stream << keyStrings.join(lit(", ")) << Qt::endl; // TODO: word wrap.
    }

    return 0;
}

int Qnob::runService(const QnobArgs& args) {
    /* Create a real logger first. */
    bool logFileOk = true;
    if (args.logPath == lit("-")) {
        m_logFile->open(2, QIODevice::WriteOnly);
    } else {
        m_logFile->setFileName(args.logPath);
        if (!m_logFile->open(QIODevice::WriteOnly | QIODevice::Append)) {
            logFileOk = false;
            m_logFile->open(2, QIODevice::WriteOnly);
        }
    }
    m_bufferLogger->flush(m_fileLogger.get());
    Logger::installGlobalLogger(m_fileLogger.get());
    if (!logFileOk)
        xthrow Exception(sformat(Exception::tr("Could not open log file '{}'."), args.logPath));

    /* Read config & populate entity pool. */
    QnobConfig config = QnobConfig::loadFromTomlFile(args.configPath);
    QString basePath = QFileInfo(args.configPath).absolutePath();

    DefaultEntityPool entityPool(this);
    DefaultEntityFactoryPool factoryPool(this);
    entityPool.loadEntities(basePath, &factoryPool, config.records);

    /*HeosDiscovery d;
    d.discoverAll();*/

    return m_application->exec();
}

