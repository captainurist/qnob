#include <fstream>

#include <QtWidgets/QApplication>

#include <core/entity/entity_factory_pool.h>
#include <core/entity/entity_pool.h>
#include <core/entity/entity_pool_builder.h>

#include <core/knob/volume_knob.h>
#include <core/trigger/hotkey_trigger_factory.h>
#include <core/sound/sound_factory.h>
#include <core/skin/skin_factory.h>
#include <core/osd/osd_factory.h>

#include <config/full_config.h>

#include <platform/platform_initializer.h>

#include <core/osd/osd_window.h>

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    PlatformInitializer platform;

    FullConfig config = FullConfig::loadFromTomlFile(QLatin1String("qnob.toml"));

    EntityPool pool;
    pool.addEntity(new VolumeKnob());

    EntityFactoryPool factoryPool;
    factoryPool.registerFactory(new HotkeyTriggerFactory());
    factoryPool.registerFactory(new SoundFactory());
    factoryPool.registerFactory(new SkinFactory());
    factoryPool.registerFactory(new OsdFactory());

    EntityPoolBuilder builder(&factoryPool, &pool);
    builder.addEntities(config);

    return application.exec();
}
