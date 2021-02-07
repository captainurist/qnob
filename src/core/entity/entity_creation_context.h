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
        Entity* result = requireEntity(key);
        return dynamic_cast<T*>(result); // TODO
    }

private:
    template<class T>
    T requireDataInternal(const QString& key, const T* defaultValue = nullptr) const;

private:
    const EntityConfig* m_config;
    FactoryResolver* m_resolver;
};
