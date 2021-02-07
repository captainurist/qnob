#include "entity_factory.h"

#include <type_traits>

#include <QtCore/QMetaType>
#include <QtCore/QVariant>
#include <QtCore/QFileInfo>

#include <config/entity_config.h>
#include <util/map_access.h>

#include "entity_creation_exception.h"
#include "factory_resolver.h"

QString EntityFactory::requireString(const EntityConfig& config, const QString& key) {
    return requireDataInternal<QString>(config, key);
}

QString EntityFactory::requireStringOr(const EntityConfig& config, const QString& key, const QString& defaultValue) {
    return requireDataInternal<QString>(config, key, &defaultValue);
}

qint64 EntityFactory::requireInt(const EntityConfig& config, const QString& key) {
    return requireDataInternal<qint64>(config, key);
}

qint64 EntityFactory::requireIntOr(const EntityConfig& config, const QString& key, qint64 defaultValue) {
    return requireDataInternal<qint64>(config, key, &defaultValue);
}

QVariantList EntityFactory::requireList(const EntityConfig& config, const QString& key) {
    return requireDataInternal<QVariantList>(config, key);
}

QVariantList EntityFactory::requireListOr(const EntityConfig& config, const QString& key, const QVariantList& defaultValue) {
    return requireDataInternal<QVariantList>(config, key, &defaultValue);
}

QString EntityFactory::requirePath(const EntityConfig& config, FactoryResolver* resolver, const QString& key) {
    QString path = resolver->resolvePath(requireString(config, key));
    if (!QFileInfo::exists(path))
        qthrow EntityCreationException(config.id, EntityCreationException::tr("File '%1' doesn't exist.").arg(path));
    return path;
}

template<class T>
static T EntityFactory::requireDataInternal(const EntityConfig& config, const QString& key, const T* defaultValue) {
    if (!config.data.contains(key)) {
        if (defaultValue) {
            return *defaultValue;
        } else {
            qthrow EntityCreationException(config.id, EntityCreationException::tr("Required parameter '%1' is not defined.").arg(key));
        }
    }

    QVariant result = value(config.data, key);
    if constexpr (std::is_same_v<T, QVariant>)
        return result;

    if (result.typeId() != qMetaTypeId<T>()) {
        qthrow EntityCreationException(
            config.id,
            EntityCreationException::tr("Invalid parameter '%1' type - expected '%2', got '%3'.")
                .arg(key)
                .arg(QLatin1String(QMetaType::fromType<T>().name()))
                .arg(QLatin1String(result.metaType().name()))
        );
    }

    return result.value<T>();
}
