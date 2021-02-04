#include "hotkey_trigger_factory.h"

#include <type_traits>

#include <config/entity_config.h>
#include <core/entity_creation_exception.h>
#include <core/entity_resolver.h>
#include <utility/map_access.h>

#include <knob/knob.h> // TODO: drop

#include "hotkey_trigger.h"

Entity* HotkeyTriggerFactory::createEntity(const EntityConfig& config, EntityResolver* resolver) {
    QKeySequence shortcut = QKeySequence::fromString(requireData<QString>(config, QStringLiteral("trigger")));
    if (shortcut.isEmpty())
        qthrow EntityCreationException(config.id, tr("TODO"));

    Entity* target = resolver->resolveEntity(requireData<QString>(config, QStringLiteral("target")));

    QString action = requireData<QString>(config, QStringLiteral("action"));

    HotkeyTrigger* result = new HotkeyTrigger(shortcut, config.id);
    connect(result, &Trigger::triggered, target, [=]() {
        if (Knob* knob = dynamic_cast<Knob*>(target))
            knob->setEnabled(!knob->enabled());
    });
    return result;
}

template<class T>
static T HotkeyTriggerFactory::requireData(const EntityConfig& config, const QString& key) {
    if (!config.data.contains(key))
        qthrow EntityCreationException(config.id, tr("Required parameter '%1' is not defined.").arg(key));

    QVariant result = value(config.data, key);
    if constexpr (std::is_same_v<T, QVariant>)
        return result;

    if (result.typeId() != qMetaTypeId<T>()) {
        // TODO
        qthrow EntityCreationException(config.id, tr("Invalid parameter '%1' type - expected '%2', got '%3'.").arg(key));
    }

    return result.value<T>();
}
