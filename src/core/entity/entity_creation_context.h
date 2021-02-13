#pragma once

#include <typeinfo>

#include <QtCore/QString>
#include <QtCore/QVariant>

#include <util/exception.h>

class Entity;
class EntityConfig;
class FactoryResolver;

class EntityCreationContext {
public:
    EntityCreationContext(const EntityConfig* config, FactoryResolver* resolver);

    const QString& id() const;

    bool has(const QString& key) const;

    QString requireString(const QString& key) const;
    QString requireStringOr(const QString& key, const QString& defaultValue) const;
    qint64 requireInt(const QString& key) const;
    qint64 requireIntOr(const QString& key, qint64 defaultValue) const;
    QVariantList requireList(const QString& key) const;
    QVariantList requireListOr(const QString& key, const QVariantList& defaultValue) const;

    QString requirePath(const QString& key) const;

    Entity* requireEntity(const QString& key) const;

    template<class T>
    T* requireEntity(const QString& key) const {
        T* result = dynamic_cast<T*>(requireEntity(key));
        if (!result)
            throwBadEntityCast(key, typeid(T));
        return result;
    }

    template<class T>
    T requireSerializable(const QString& key) const {
        QString string = requireString(key);
        T result;
        try {
            deserialize(string, &result);
        } catch (const Exception&) {
            throwDeserializationError(key); /* Exception will auto-chain! */
        }
        return result;
    }

    template<class T>
    T requireSerializableOr(const QString& key, const T& defaultValue) const {
        if (!has(key))
            return defaultValue;
        return requireSerializable<T>(key);
    }

private:
    template<class T>
    T requireDataInternal(const QString& key, const T* defaultValue = nullptr) const;

    [[noreturn]] void throwBadEntityCast(const QString& key, const std::type_info& typeInfo) const;
    [[noreturn]] void throwDeserializationError(const QString& key) const;

private:
    const EntityConfig* m_config;
    FactoryResolver* m_resolver;
};
