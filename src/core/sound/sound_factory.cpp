#include "sound_factory.h"

#include <core/setting/setting.h>

#include "sound.h"

SoundFactory::SoundFactory():
    EntityFactory(lit("sound"))
{}

Entity* SoundFactory::createEntity(const EntityCreationContext& ctx) {
    QString path = ctx.require<QString, AsPath>(lit("path"));
    Setting* target = ctx.require<Setting*>(lit("target"));

    Sound* result = new Sound(ctx.id(), path);
    QObject::connect(target, &Setting::activated, result, &Sound::play);
    return result;
}

