#include <fstream>

#include <QtCore/QThread>
#include <QtWidgets/QApplication>

#include <core/entity/entity_factory_pool.h>
#include <core/entity/entity_pool.h>
#include <core/entity/entity_pool_builder.h>

#include <core/knob/knob.h>
#include <core/knob/volume_shaft.h>
#include <core/trigger/hotkey_trigger_factory.h>
#include <core/sound/sound_factory.h>
#include <core/skin/skin_factory.h>
#include <core/osd/osd_factory.h>

#include <config/full_config.h>

#include <platform/platform_initializer.h>

#include <core/osd/osd_window.h>

int main(int argc, char *argv[]) {
    try {
        QApplication application(argc, argv);
        QApplication::setQuitOnLastWindowClosed(false);
        QThread::currentThread()->setObjectName(lit("main"));

        PlatformInitializer platform;

        FullConfig config = FullConfig::loadFromTomlFile(QLatin1String("qnob.toml"));

        EntityPool pool;
        pool.addEntity(new Knob(lit("volume"), new VolumeShaft()));

        EntityFactoryPool factoryPool;
        factoryPool.registerFactory(new HotkeyTriggerFactory());
        factoryPool.registerFactory(new SoundFactory());
        factoryPool.registerFactory(new SkinFactory());
        factoryPool.registerFactory(new OsdFactory());

        EntityPoolBuilder builder(&factoryPool, &pool);
        builder.addEntities(config);

        return application.exec();
    } catch (const Exception& e) {
        qCritical() << e;
        return 1;
    }
}
