#include "hotkey_trigger_factory.h"

#include <type_traits>

#include <config/entity_config.h>
#include <core/entity/entity_creation_exception.h>
#include <core/entity/entity_resolver.h>
#include <util/map_access.h>
#include <meta/bound_meta_call.h>

#include "hotkey_trigger.h"

HotkeyTriggerFactory::HotkeyTriggerFactory() :
    EntityFactory(QStringLiteral("shortcut"))
{}

Entity* HotkeyTriggerFactory::createEntity(const EntityConfig& config, EntityResolver* resolver) {
    QKeySequence shortcut = QKeySequence::fromString(requireData<QString>(config, QStringLiteral("trigger")));
    if (shortcut.isEmpty())
        qthrow EntityCreationException(config.id, EntityCreationException::tr("TODO"));

    Entity* target = resolver->resolveEntity(requireData<QString>(config, QStringLiteral("target")));
    QString action = requireData<QString>(config, QStringLiteral("action"));
    QVariantList args = requireDataOr<QVariantList>(config, QStringLiteral("args"), QVariantList());

    BoundMetaCall call;
    call.bind(target, action.toUtf8(), args);

    HotkeyTrigger* result = new HotkeyTrigger(shortcut, config.id);
    QObject::connect(result, &Trigger::triggered, target, [=]() mutable {
        call.invoke();
    });
    return result;
}

