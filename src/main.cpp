#include <fstream>

#include <QtCore/QThread>
#include <QtWidgets/QApplication>

#include <core/entity/entity_factory_pool.h>
#include <core/entity/entity_pool.h>
#include <core/entity/entity_pool_builder.h>

#include <core/app/app.h>
#include <core/knob/knob.h>
#include <core/knob/brightness_shaft.h>
#include <core/knob/volume_shaft.h>
#include <core/trigger/hotkey_trigger_factory.h>
#include <core/sound/sound_factory.h>
#include <core/skin/skin_factory.h>
#include <core/osd/osd_factory.h>
#include <core/tray_icon/tray_icon_factory.h>

#include <config/full_config.h>

#include <platform/platform_initializer.h>

int main(int argc, char *argv[]) {
    try {
        QApplication application(argc, argv);
        QApplication::setQuitOnLastWindowClosed(false);
        QThread::currentThread()->setObjectName(lit("MainThread"));

        PlatformInitializer platform;

        EntityPool pool;
        pool.addEntity(new Knob(lit("volume"), new VolumeShaft()));
        pool.addEntity(new Knob(lit("brightness"), new BrightnessShaft()));
        pool.addEntity(new App(lit("app")));

        EntityFactoryPool factoryPool;
        factoryPool.registerFactory(new HotkeyTriggerFactory());
        factoryPool.registerFactory(new SoundFactory());
        factoryPool.registerFactory(new SkinFactory(HorizontalBarSkinType));
        factoryPool.registerFactory(new OsdFactory());
        factoryPool.registerFactory(new TrayIconFactory());

        EntityPoolBuilder builder(&factoryPool, &pool);
        builder.addEntities(FullConfig::loadFromTomlFile(QLatin1String("qnob.toml")));

        return application.exec();
    } catch (const Exception& e) {
        qCritical() << e;
        return 1;
    }
}
