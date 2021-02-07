#include "hotkey_trigger_factory.h"

#include <type_traits>

#include <config/entity_config.h>
#include <core/entity/entity_creation_exception.h>
#include <core/entity/entity_resolver.h>
#include <util/map_access.h>
#include <meta/bound_meta_call.h>

#include "hotkey_trigger.h"

HotkeyTriggerFactory::HotkeyTriggerFactory() :
    EntityFactory(lit("shortcut"))
{}

Entity* HotkeyTriggerFactory::createEntity(const EntityConfig& config, EntityResolver* resolver) {
    QKeySequence shortcut = QKeySequence::fromString(requireData<QString>(config, lit("trigger")));
    if (shortcut.isEmpty())
        qthrow EntityCreationException(config.id, EntityCreationException::tr("TODO"));

    Entity* target = resolver->resolveEntity(requireData<QString>(config, lit("target")));
    QString action = requireData<QString>(config, lit("action"));
    QVariantList args = requireDataOr<QVariantList>(config, lit("args"), QVariantList());

    BoundMetaCall call;
    call.bind(target, action.toUtf8(), args);

    HotkeyTrigger* result = new HotkeyTrigger(config.id, shortcut);
    QObject::connect(result, &Trigger::triggered, target, [=]() mutable {
        call.invoke();
    });
    return result;
}

