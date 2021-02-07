#include "sound_factory.h"

#include <config/entity_config.h>
#include <core/entity/factory_resolver.h>

#include "sound.h"

SoundFactory::SoundFactory():
    EntityFactory(lit("sound"))
{}

Entity* SoundFactory::createEntity(const EntityConfig& config, FactoryResolver* resolver) {
    QString path = requirePath(config, resolver, lit("path"));

    Entity* target = resolver->resolveEntity(requireString(config, lit("target")));

    Sound* result = new Sound(config.id, path);

    QObject::connect(target, SIGNAL(touched()), result, SLOT(play()));

    return result;
}

