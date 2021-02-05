#include <fstream>

#include <QtWidgets/QApplication>

#include <core/entity/entity_factory_pool.h>
#include <core/entity/entity_pool.h>
#include <core/entity/entity_pool_builder.h>

#include <core/knob/volume_knob.h>
#include <core/trigger/hotkey_trigger_factory.h>

#include <config/full_config.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    FullConfig config = FullConfig::loadFromTomlFile(QLatin1String("qnob.toml"));

    EntityPool pool;
    pool.addEntity(new VolumeKnob());

    EntityFactoryPool factoryPool;
    factoryPool.registerFactory(new HotkeyTriggerFactory());

    EntityPoolBuilder builder(&factoryPool, &pool);
    builder.addEntities(config);

    return a.exec();
}
