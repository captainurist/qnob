#include "entity_factory.h"

#include <type_traits>

#include <QtCore/QMetaType>
#include <QtCore/QVariant>

#include <config/entity_config.h>
#include <util/map_access.h>

#include "entity_creation_exception.h"

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

/* We only need a couple instantiations, so here they are. */
#define EF_INSTANTIATE(TYPE) \
template TYPE EntityFactory::requireDataInternal<TYPE>(const EntityConfig&, const QString&, const TYPE*);

EF_INSTANTIATE(bool)
EF_INSTANTIATE(int64_t)
EF_INSTANTIATE(double)
EF_INSTANTIATE(QString)
EF_INSTANTIATE(QVariant)
EF_INSTANTIATE(QVariantList)
