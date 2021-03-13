#include "entity_creation_context.h"

#include <config/entity_config.h>
#include <core/entity/entity_creation_exception.h>
#include <util/map_access.h>

EntityCreationContext::EntityCreationContext(const EntityConfig* config, FactoryResolver* resolver):
    m_config(config),
    m_resolver(resolver)
{}

const QString& EntityCreationContext::id() const {
    return m_config->id;
}

FactoryResolver* EntityCreationContext::resolver() const {
    return m_resolver;
}

bool EntityCreationContext::has(const QString& key) const {
    return m_config->data.contains(key);
}

QVariant EntityCreationContext::require(const QString& key) const {
    QVariant result = requireOr(key, QVariant());
    if (result.isNull())
        qthrow EntityCreationException(id(), EntityCreationException::tr("Required parameter '%1' is not defined.").arg(key));
    return result;
}

QVariant EntityCreationContext::requireOr(const QString& key, const QVariant& defaultValue) const {
    return value_or(m_config->data, key, defaultValue);
}

void EntityCreationContext::throwCreationException(const QString& key) const {
    qthrow EntityCreationException(
        id(),
        EntityCreationException::tr("Parameter '%1' has invalid value.").arg(key)
    );
}
