#include <QtCore/QThreadPool>
#include <QtCore/QScopeGuard>
#include <QtWidgets/QApplication>

#include <lib/command_line/command_line_parser.h>
#include <lib/command_line/command_line_exception.h>

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

#include <config/qnob_config.h>
#include <config/qnob_args.h>

#include <platform/platform_initializer.h>
#include <platform/platform.h>

void maybePressAnyKey() {
    if (platform()->execute(WinIsConsoleOwned).toBool()) {
        QTextStream stream(stdout);
        stream << lit("[press any key to close this window]") << Qt::endl;
        fgetc(stdin);
    }
}

bool processCommandLine(const QStringList& args, QnobArgs* params) {
    CommandLineParser parser;

    bool help = false;
    bool version = false;

    // TODO: tr

    parser.addSection(QString());
    parser.addOption("config", lit("Path to a config file.")).argument("PATH", &params->configPath).defaultValue(lit("./qnob.toml"));
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

int main(int argc, char* argv[]) {
    /* Qt & platform classes don't throw. */
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

        EntityPoolBuilder builder(&factoryPool, &pool);
        builder.addEntities(QnobConfig::loadFromTomlFile(args.configPath));

        platform()->execute(WinEnableHooks);
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
