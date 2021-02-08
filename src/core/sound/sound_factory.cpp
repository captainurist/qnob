#include "sound_factory.h"

#include <config/entity_config.h>
#include <core/entity/factory_resolver.h>
#include <core/knob/knob.h>

#include "sound.h"

SoundFactory::SoundFactory():
    EntityFactory(lit("sound"))
{}

Entity* SoundFactory::createEntity(const EntityCreationContext& ctx) {
    QString path = ctx.requirePath(lit("path"));
    Knob* target = ctx.requireEntity<Knob>(lit("target"));

    Sound* result = new Sound(ctx.id(), path);

    QObject::connect(target, &Knob::activated, result, &Sound::play);

    return result;
}

