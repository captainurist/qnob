#include "entity_creation_context.h"

#include <core/entity/entity_creation_exception.h>
#include <util/map_access.h>

EntityCreationContext::EntityCreationContext(const QString& id, const VariantMap& config, FactoryResolver* resolver):
    m_id(id),
    m_config(config),
    m_resolver(resolver)
{}

const QString& EntityCreationContext::id() const {
    return m_id;
}

FactoryResolver* EntityCreationContext::resolver() const {
    return m_resolver;
}

bool EntityCreationContext::has(const QString& key) const {
    return m_config.contains(key);
}

QVariant EntityCreationContext::require(const QString& key) const {
    QVariant result = requireOr(key, QVariant());
    if (result.isNull())
        xthrow EntityCreationException(id(), EntityCreationException::tr("Required parameter '%1' is not defined.").arg(key));
    return result;
}

QVariant EntityCreationContext::requireOr(const QString& key, const QVariant& defaultValue) const {
    return value_or(m_config, key, defaultValue);
}

void EntityCreationContext::throwCreationException(const QString& key) const {
    xthrow EntityCreationException(
        id(),
        EntityCreationException::tr("Parameter '%1' has invalid value.").arg(key)
    );
}
