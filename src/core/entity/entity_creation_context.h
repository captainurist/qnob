#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>

class Entity;
class EntityConfig;
class FactoryResolver;

class EntityCreationContext {
public:
    EntityCreationContext(const EntityConfig* config, FactoryResolver* resolver);

    const QString& id() const;

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
            throwBadEntityCast(key, QMetaType::fromType<T>());
        return result;
    }

private:
    template<class T>
    T requireDataInternal(const QString& key, const T* defaultValue = nullptr) const;

    [[noreturn]] void throwBadEntityCast(const QString& key, const QMetaType& type) const;

private:
    const EntityConfig* m_config;
    FactoryResolver* m_resolver;
};
