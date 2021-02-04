#include <fstream>

#include <QtWidgets/QApplication>

#include <core/entity_factory_pool.h>
#include <core/entity_pool.h>
#include <core/entity_pool_builder.h>

#include <knob/volume_knob.h>
#include <trigger/hotkey_trigger_factory.h>

#include <utility/exception.h>

#include "config/full_config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //PlatformShortcutNotifier* notifier = qPlatform()->createShortcutNotifier(QKeySequence(QLatin1String("Shift+Pause")));

    //QObject::connect(notifier, &PlatformShortcutNotifier::activated, [] { qDebug() << "!!!!1"; });

    FullConfig config = FullConfig::loadFromTomlFile(QLatin1String("qnob.toml"));

    EntityPool pool;
    pool.addEntity(new VolumeKnob());

    EntityFactoryPool factoryPool;
    factoryPool.registerFactory(QLatin1String("shortcut"), new HotkeyTriggerFactory()); // TODO: ID inside!

    EntityPoolBuilder builder(&factoryPool, &pool);
    builder.addEntities(config);

    return a.exec();
}
