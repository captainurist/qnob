#include "hotkey_factory.h"

#include <lib/metacall/bound_meta_call.h>
#include <lib/keys/key_combination.h>

#include "hotkey.h"

HotkeyFactory::HotkeyFactory() :
    EntityFactory(lit("hotkey"))
{}

Entity* HotkeyFactory::createEntity(const EntityCreationContext& ctx) {
    QKeyCombination shortcut = ctx.require<QKeyCombination>(lit("trigger"));
    Entity* target = ctx.require<Entity*>(lit("target"));
    QString action = ctx.require<QString>(lit("action"));
    QVariantList args = ctx.requireOr<QVariantList>(lit("args"), QVariantList());

    BoundMetaCall call;
    call.bind(target, action.toUtf8(), args);

    Hotkey* result = new Hotkey(ctx.id(), shortcut);
    QObject::connect(result, &Hotkey::triggered, target, [=]() mutable {
        call.invoke();
    });
    return result;
}

