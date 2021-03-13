#include "hotkey_trigger_factory.h"

#include <lib/metacall/bound_meta_call.h>

#include "hotkey_trigger.h"

HotkeyTriggerFactory::HotkeyTriggerFactory() :
    EntityFactory(lit("shortcut"))
{}

Entity* HotkeyTriggerFactory::createEntity(const EntityCreationContext& ctx) {
    QKeySequence shortcut = ctx.require<QKeySequence>(lit("trigger"));
    Entity* target = ctx.require<Entity*>(lit("target"));
    QString action = ctx.require<QString>(lit("action"));
    QVariantList args = ctx.requireOr<QVariantList>(lit("args"), QVariantList());

    BoundMetaCall call;
    call.bind(target, action.toUtf8(), args);

    HotkeyTrigger* result = new HotkeyTrigger(ctx.id(), shortcut);
    QObject::connect(result, &Trigger::triggered, target, [=]() mutable {
        call.invoke();
    });
    return result;
}

