#include "qnob.h"

#include <ranges>

#include <QtCore/QThreadPool>
#include <QtCore/QScopeGuard>
#include <QtCore/QFileInfo>
#include <QtWidgets/QApplication>

#include <core/entity/entity_pool_builder.h>

#include <lib/command_line/command_line_parser.h>
#include <lib/command_line/command_line_exception.h>
#include <lib/logging/logger.h>
#include <lib/logging/buffer_logger.h>
#include <lib/logging/file_logger.h>
#include <lib/serializable_types/key_combination.h>

#include <platform/platform_initializer.h>
#include <platform/platform.h>

#include "qnob_config.h"
#include "qnob_args.h"
#include "qnob_command_line_parser.h"
#include "default_entity_pool.h"
#include "default_entity_factory_pool.h"

Qnob::Qnob() {
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

    /* Init logging first. Calls after this one might log! */
    Logger::installGlobalLogger(m_bufferLogger.get());

    /* Qt & platform classes don't throw, and they are used inside catch blocks.
     * So we initialize them here. */
    m_application.reset(new QApplication(argc, argv));
    QApplication::setQuitOnLastWindowClosed(false);
    QThread::currentThread()->setObjectName(lit("MainThread"));
    PlatformInitializer platformInitializer;

    /* Join all worker threads before platform is destroyed, there might be some deinitialization pending there. */
    auto cleanup = QScopeGuard([] { QThreadPool::globalInstance()->waitForDone(); });

    try {
        QnobCommandLineParser parser;
        return run(parser.parse(m_application->arguments()));
    } catch (const CommandLineException& e) {
        platform()->execute(WinEnsureConsole);
        QTextStream stream(stderr);
        stream << e.message() << Qt::endl;
        stream << tr("Try 'qnob --help' for more information.");
        maybePressAnyKey();
        return 1;
    } catch (const Exception& e) {
        qCritical() << e;
        maybePressAnyKey();
        return 1;
    }
}

int Qnob::run(const QnobArgs& args) {
    if (args.console)
        platform()->execute(WinEnsureConsole);

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

    stream << tr("Usage: qnob [options]") << Qt::endl;  // TODO: tr
    stream << Qt::endl;

    size_t consoleWidth = 80;
    if (QSize consoleSize = platform()->execute(GetConsoleSize).toSize(); consoleSize.isValid())
        consoleWidth = std::max(consoleSize.width(), 40);

    CommandLineHelpOptions options;
    options.maxLineLength = consoleWidth;

    QnobCommandLineParser parser;
    parser.printSections(stream, options);

    maybePressAnyKey();
    return 0;
}

int Qnob::runVersion() {
    QTextStream stream(stdout);

    stream << tr("qnob v%1").arg(lit(QNOB_VERSION_STRING)) << Qt::endl;

    maybePressAnyKey();
    return 0;
}

int Qnob::runList(const QnobArgs& args) {
    QTextStream stream(stdout);

    if (args.list == EntitiesList) {
        DefaultEntityPool entityPool;

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
                keyStrings.push_back(lit("'%1'").arg(serialized(key)));
        keyStrings.sort(Qt::CaseInsensitive);

        stream << tr("Available keys:") << Qt::endl;
        stream << keyStrings.join(lit(", ")) << Qt::endl; // TODO: word wrap.
    }

    maybePressAnyKey();
    return 0;
}

int Qnob::runService(const QnobArgs& args) {
    /* Create a real logger first. */
    bool logFileOk = true;
    if (args.console) {
        m_logFile->open(2, QIODevice::WriteOnly);
    } else {
        m_logFile->setFileName(args.logPath);
        if (!m_logFile->open(QIODevice::WriteOnly | QIODevice::Append)) {
            logFileOk = false;
            platform()->execute(WinEnsureConsole);
            m_logFile->open(2, QIODevice::WriteOnly);
        }
    }
    m_bufferLogger->flush(m_fileLogger.get());
    Logger::installGlobalLogger(m_fileLogger.get());
    if (!logFileOk)
        qthrow Exception(Exception::tr("Could not open log file '%1'.").arg(args.logPath));

    /* Read config & populate entity pool. */
    QnobConfig config = QnobConfig::loadFromTomlFile(args.configPath);
    QString basePath = QFileInfo(args.configPath).absolutePath();

    DefaultEntityPool entityPool;
    DefaultEntityFactoryPool factoryPool;
    EntityPoolBuilder builder(&factoryPool, &entityPool);
    builder.addEntities(basePath, config.records);

    return m_application->exec();
}

void Qnob::maybePressAnyKey() {
    if (platform()->execute(WinIsConsoleOwned).toBool()) {
        QTextStream stream(stdout);
        stream << tr("[press any key to close this window]") << Qt::endl;
        fgetc(stdin);
    }
}
