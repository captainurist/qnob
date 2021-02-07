#include "sound_factory.h"

#include <config/entity_config.h>
#include <core/entity/entity_resolver.h>

#include "sound.h"

SoundFactory::SoundFactory():
    EntityFactory(lit("sound"))
{}

Entity* SoundFactory::createEntity(const EntityConfig& config, EntityResolver* resolver) {
    QString path = requireData<QString>(config, lit("path"));

    Entity* target = resolver->resolveEntity(requireData<QString>(config, lit("target")));

    Sound* result = new Sound(config.id, path);

    QObject::connect(target, SIGNAL(touched()), result, SLOT(play()));

    return result;
}

