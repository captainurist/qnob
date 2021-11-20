#include "entity_config.h"

#include <core/entity/entity_creation_exception.h>
#include <util/map_access.h>

EntityConfig::EntityConfig(const QString& id, const VariantMap& config, const QString& basePath, const EntityPool* entityPool):
    m_id(id),
    m_config(config),
    m_basePath(basePath),
    m_entityPool(entityPool)
{}

const QString& EntityConfig::id() const {
    return m_id;
}

const EntityPool* EntityConfig::entityPool() const {
    return m_entityPool;
}

const QString& EntityConfig::basePath() const {
    return m_basePath;
}

bool EntityConfig::has(const QString& key) const {
    return m_config.contains(key);
}

QVariant EntityConfig::require(const QString& key) const {
    QVariant result = requireOr(key, QVariant());
    if (result.isNull())
        xthrow EntityCreationException(id(), EntityCreationException::tr("Required parameter '%1' is not defined.").arg(key));
    return result;
}

QVariant EntityConfig::requireOr(const QString& key, const QVariant& defaultValue) const {
    return value_or(m_config, key, defaultValue);
}

void EntityConfig::throwCreationException(const QString& key) const {
    xthrow EntityCreationException(
        id(),
        EntityCreationException::tr("Parameter '%1' has invalid value.").arg(key)
    );
}
