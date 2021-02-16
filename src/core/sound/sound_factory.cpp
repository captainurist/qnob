#include "sound_factory.h"

#include <config/entity_config.h>
#include <core/entity/factory_resolver.h>
#include <core/knob/knob.h>

#include "sound.h"

SoundFactory::SoundFactory():
    EntityFactory(lit("sound"))
{}

Entity* SoundFactory::createEntity(const EntityCreationContext& ctx) {
    QString path = ctx.require<QString, AsPath>(lit("path"));
    Knob* target = ctx.requireOr<Knob*>(lit("target"), nullptr);
    std::vector<Knob*> targets = ctx.requireOr<std::vector<Knob*>>(lit("targets"), std::vector<Knob*>());
    if (target)
        targets.push_back(target);

    Sound* result = new Sound(ctx.id(), path);

    for (Knob* knob : targets)
        QObject::connect(knob, &Knob::activated, result, &Sound::play);

    return result;
}

