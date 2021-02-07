#include "sound_factory.h"

#include <config/entity_config.h>
#include <core/entity/factory_resolver.h>

#include "sound.h"

SoundFactory::SoundFactory():
    EntityFactory(lit("sound"))
{}

Entity* SoundFactory::createEntity(const EntityCreationContext& ctx) {
    QString path = ctx.requirePath(lit("path"));
    Entity* target = ctx.requireEntity(lit("target"));

    Sound* result = new Sound(ctx.id(), path);

    QObject::connect(target, SIGNAL(touched()), result, SLOT(play()));

    return result;
}

