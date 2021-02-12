#include "hotkey_trigger_factory.h"

#include <type_traits>

#include <config/entity_config.h>
#include <core/entity/entity_creation_exception.h>
#include <core/entity/factory_resolver.h>
#include <util/map_access.h>
#include <meta/bound_meta_call.h>

#include "hotkey_trigger.h"

HotkeyTriggerFactory::HotkeyTriggerFactory() :
    EntityFactory(lit("shortcut"))
{}

Entity* HotkeyTriggerFactory::createEntity(const EntityCreationContext& ctx) {
    QString shortcutString = ctx.requireString(lit("trigger"));
    QKeySequence shortcut = QKeySequence::fromString(shortcutString);
    if (shortcut.isEmpty() || shortcut[0].key() == Qt::Key_unknown)
        qthrow EntityCreationException(ctx.id(), EntityCreationException::tr("String '%1' does not define a valid key sequence.").arg(shortcutString));

    Entity* target = ctx.requireEntity(lit("target"));
    QString action = ctx.requireString(lit("action"));
    QVariantList args = ctx.requireListOr(lit("args"), QVariantList());

    BoundMetaCall call;
    call.bind(target, action.toUtf8(), args);

    HotkeyTrigger* result = new HotkeyTrigger(ctx.id(), shortcut);
    QObject::connect(result, &Trigger::triggered, target, [=]() mutable {
        call.invoke();
    });
    return result;
}

