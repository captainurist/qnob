#include "entity_creation_context.h"

#include <core/entity/entity_creation_exception.h>
#include <util/map_access.h>

EntityCreationContext::EntityCreationContext(const QString& id, const VariantMap& config, const QString& basePath, const EntityPool* entityPool):
    m_id(id),
    m_config(config),
    m_basePath(basePath),
    m_entityPool(entityPool)
{}

const QString& EntityCreationContext::id() const {
    return m_id;
}

const EntityPool* EntityCreationContext::entityPool() const {
    return m_entityPool;
}

const QString& EntityCreationContext::basePath() const {
    return m_basePath;
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
