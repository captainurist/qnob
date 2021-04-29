#include "qnob.h"

#include <QtCore/QThreadPool>
#include <QtCore/QScopeGuard>
#include <QtCore/QFileInfo>
#include <QtWidgets/QApplication>

#include <lib/command_line/command_line_parser.h>
#include <lib/command_line/command_line_exception.h>
#include <lib/logging/logger.h>
#include <lib/logging/buffer_logger.h>
#include <lib/logging/file_logger.h>

#include <core/entity/entity_factory_pool.h>
#include <core/entity/entity_pool.h>
#include <core/entity/entity_pool_builder.h>
#include <core/app/app.h>
#include <core/knob/knob_factory.h>
#include <core/setting/brightness_setting_backend.h>
#include <core/setting/volume_setting_backend.h>
#include <core/setting/setting.h>
#include <core/hotkey/hotkey_factory.h>
#include <core/sound/sound_factory.h>
#include <core/skin/skin_factory.h>
#include <core/osd/osd_factory.h>
#include <core/tray_icon/tray_icon_factory.h>
#include <core/tray_icon/standard_tray_icon.h>
#include <core/tray_event/tray_event_factory.h>

#include <platform/platform_initializer.h>
#include <platform/platform.h>

#include "qnob_config.h"
#include "qnob_args.h"

static void maybePressAnyKey() {
    if (platform()->execute(WinIsConsoleOwned).toBool()) {
        QTextStream stream(stdout);
        stream << lit("[press any key to close this window]") << Qt::endl;
        fgetc(stdin);
    }
}

static bool processCommandLine(const QStringList& args, QnobArgs* params) {
    CommandLineParser parser;

    bool help = false;
    bool version = false;

    // TODO: tr

    parser.addSection(QString());
    parser.addOption("config", lit("Path to a config file.")).argument("PATH", &params->configPath).defaultValue(lit("./qnob.toml"));
    parser.addOption("log", lit("Path to a log file.")).argument("PATH", &params->logPath).defaultValue(lit("./qnob.log"));
    parser.addOption("console", lit("Always opens a console window (Windows only).")).flag(&params->console);
    parser.addOption("help", lit("Show help and exit.")).flag(&help);
    parser.addOption('v', "version", lit("Show version information and exit.")).flag(&version);
    parser.parse(args);

    if (help || version) {
        platform()->execute(WinEnsureConsole);
        QTextStream stream(stdout);

        if (help) {
            stream << lit("Usage: qnob [options]") << Qt::endl;  // TODO: tr
            stream << Qt::endl;

            size_t consoleWidth = 80;
            if (QSize consoleSize = platform()->execute(GetConsoleSize).toSize(); consoleSize.isValid())
                consoleWidth = std::max(consoleSize.width(), 40);

            CommandLineHelpOptions options;
            options.maxLineLength = consoleWidth;
            parser.printSections(stream, options);
        } else {
            stream << lit("qnob v0.0.1") << Qt::endl;
        }

        return true;
    }

    return false;
}

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
    QApplication application(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
    QThread::currentThread()->setObjectName(lit("MainThread"));
    PlatformInitializer platformInitializer;

    try {
        QnobArgs args;
        if (processCommandLine(application.arguments(), &args)) {
            maybePressAnyKey();
            return 0;
        }

        if (args.console)
            platform()->execute(WinEnsureConsole);

        /* Now that we're done with console we might create a real logger. */
        if (args.console) {
            m_logFile->open(2, QIODevice::WriteOnly);
        } else {
            m_logFile->setFileName(args.logPath);
            m_logFile->open(QIODevice::WriteOnly | QIODevice::Append);
        }
        m_bufferLogger->flush(m_fileLogger.get());
        Logger::installGlobalLogger(m_fileLogger.get());

        /* Join all worker threads before platform is destroyed, there might be some deinitialization pending there. */
        auto cleanup = QScopeGuard([] { QThreadPool::globalInstance()->waitForDone(); });

        Setting* volume = new Setting(lit("volume"), new VolumeSettingBackend());

        EntityPool pool;
        pool.addEntity(volume);
        pool.addEntity(new Setting(lit("brightness"), new BrightnessSettingBackend()));
        pool.addEntity(new App(lit("app")));
        pool.addEntity(new StandardTrayIcon(lit("volume_icon"), volume, AudioTrayIcon));

        EntityFactoryPool factoryPool;
        factoryPool.registerFactory(new KnobFactory());
        factoryPool.registerFactory(new HotkeyFactory());
        factoryPool.registerFactory(new SoundFactory());
        factoryPool.registerFactory(new SkinFactory(HorizontalBarSkinType));
        factoryPool.registerFactory(new SkinFactory(NumericSkinType));
        factoryPool.registerFactory(new OsdFactory());
        factoryPool.registerFactory(new TrayIconFactory());
        factoryPool.registerFactory(new TrayEventFactory());

        QnobConfig config = QnobConfig::loadFromTomlFile(args.configPath);
        QString basePath = QFileInfo(args.configPath).absolutePath();

        EntityPoolBuilder builder(&factoryPool, &pool);
        builder.addEntities(basePath, config.records);

        return application.exec();
    } catch (const CommandLineException& e) {
        QTextStream stream(stderr);
        stream << e.message() << Qt::endl;
        stream << lit("Try 'qnob --help' for more information."); // TODO: tr
        maybePressAnyKey();
        return 1;
    } catch (const Exception& e) {
        qCritical() << e;
        maybePressAnyKey();
        return 1;
    }
}
