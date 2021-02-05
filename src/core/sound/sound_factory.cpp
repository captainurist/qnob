#include "sound_factory.h"

#include <config/entity_config.h>
#include <core/entity/entity_resolver.h>

#include "sound.h"

SoundFactory::SoundFactory():
    EntityFactory(QStringLiteral("sound"))
{}

Entity* SoundFactory::createEntity(const EntityConfig& config, EntityResolver* resolver) {
    QString path = requireData<QString>(config, QStringLiteral("path"));

    Entity* target = resolver->resolveEntity(requireData<QString>(config, QStringLiteral("target")));

    Sound* result = new Sound(config.id, path);

    QObject::connect(target, SIGNAL(touched()), result, SLOT(play()));

    return result;
}

