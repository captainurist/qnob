#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>

#include <util/variant.h>

#include "factory_resolver.h"

class EntityConfig;

class EntityCreationContext {
public:
    EntityCreationContext(const QString& id, const VariantMap& config, FactoryResolver* resolver);

    const QString& id() const;
    FactoryResolver* resolver() const;

    bool has(const QString& key) const;
    QVariant require(const QString& key) const;
    QVariant requireOr(const QString& key, const QVariant& defaultValue) const;

    template<class T, class ValidationTag = nullptr_t>
    T require(const QString& key) const {
        return parseInternal<T, ValidationTag>(key, require(key));
    }

    template<class T, class ValidationTag = nullptr_t>
    T requireOr(const QString& key, const T& defaultValue) const {
        QVariant tmp = requireOr(key, QVariant());
        return tmp.isNull() ? defaultValue : parseInternal<T, ValidationTag>(key, tmp);
    }

private:
    template<class T, class ValidationTag>
    T parseInternal(const QString& key, const QVariant& value) const {
        T result;
        try {
            parseConfigValue(this, value, &result, ValidationTag());
        } catch (...) {
            throwCreationException(key);
        }
        return result;
    }

    [[noreturn]] void throwCreationException(const QString& key) const;

private:
    const QString& m_id;
    const VariantMap& m_config;
    FactoryResolver* m_resolver;
};

#include "entity_parsers.h"
