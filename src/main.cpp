#include <fstream>

#include <QtCore/QThread>
#include <QtWidgets/QApplication>

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

#include <config/full_config.h>

#include <platform/platform_initializer.h>

int main(int argc, char* argv[]) {
    try {
        QApplication application(argc, argv);
        QApplication::setQuitOnLastWindowClosed(false);
        QThread::currentThread()->setObjectName(lit("MainThread"));

        PlatformInitializer platform;

        EntityPool pool;
        pool.addEntity(new Setting(lit("volume"), new VolumeSettingBackend()));
        pool.addEntity(new Setting(lit("brightness"), new BrightnessSettingBackend()));
        pool.addEntity(new App(lit("app")));

        EntityFactoryPool factoryPool;
        factoryPool.registerFactory(new KnobFactory());
        factoryPool.registerFactory(new HotkeyFactory());
        factoryPool.registerFactory(new SoundFactory());
        factoryPool.registerFactory(new SkinFactory(HorizontalBarSkinType));
        factoryPool.registerFactory(new SkinFactory(NumericSkinType));
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
