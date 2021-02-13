#include "entity_creation_context.h"

#include <QtCore/QFileInfo>

#include <config/entity_config.h>
#include <core/entity/entity_creation_exception.h>
#include <core/entity/factory_resolver.h>
#include <util/human_readable_name.h>
#include <util/map_access.h>

EntityCreationContext::EntityCreationContext(const EntityConfig* config, FactoryResolver* resolver):
    m_config(config),
    m_resolver(resolver)
{}

const QString& EntityCreationContext::id() const {
    return m_config->id;
}

bool EntityCreationContext::has(const QString& key) const {
    return m_config->data.contains(key);
}

QString EntityCreationContext::requireString(const QString& key) const {
    return requireDataInternal<QString>(key);
}

QString EntityCreationContext::requireStringOr(const QString& key, const QString& defaultValue) const {
    return requireDataInternal<QString>(key, &defaultValue);
}

qint64 EntityCreationContext::requireInt(const QString& key) const {
    return requireDataInternal<qint64>(key);
}

qint64 EntityCreationContext::requireIntOr(const QString& key, qint64 defaultValue) const {
    return requireDataInternal<qint64>(key, &defaultValue);
}

QVariantList EntityCreationContext::requireList(const QString& key) const {
    return requireDataInternal<QVariantList>(key);
}

QVariantList EntityCreationContext::requireListOr(const QString& key, const QVariantList& defaultValue) const {
    return requireDataInternal<QVariantList>(key, &defaultValue);
}

Entity* EntityCreationContext::requireEntity(const QString& key) const {
    return m_resolver->resolveEntity(requireString(key));
}

QString EntityCreationContext::requirePath(const QString& key) const {
    QString path = m_resolver->resolvePath(requireString(key));
    if (!QFileInfo::exists(path))
        qthrow EntityCreationException(id(), EntityCreationException::tr("File '%1' doesn't exist.").arg(path));
    return path;
}

template<class T>
T EntityCreationContext::requireDataInternal(const QString& key, const T* defaultValue) const {
    if (!m_config->data.contains(key)) {
        if (defaultValue) {
            return *defaultValue;
        } else {
            qthrow EntityCreationException(id(), EntityCreationException::tr("Required parameter '%1' is not defined.").arg(key));
        }
    }

    QVariant result = value_or(m_config->data, key);
    if constexpr (std::is_same_v<T, QVariant>)
        return result;

    if (result.typeId() != qMetaTypeId<T>()) {
        qthrow EntityCreationException(
            id(),
            EntityCreationException::tr("Invalid parameter '%1' type - expected '%2', got '%3'.")
                .arg(key)
                .arg(humanReadableName<T>())
                .arg(QLatin1String(result.metaType().name()))
        );
    }

    return result.value<T>();
}

void EntityCreationContext::throwBadEntityCast(const QString& key, const std::type_info& typeInfo) const {
    qthrow EntityCreationException(
        id(),
        EntityCreationException::tr("Invalid type of '%1' entity - expected '%2'.").arg(key).arg(humanReadableName(typeInfo))
    );
}

void EntityCreationContext::throwDeserializationError(const QString& key) const {
    qthrow EntityCreationException(
        id(),
        EntityCreationException::tr("Invalid parameter '%1' value.").arg(key)
    );
}
