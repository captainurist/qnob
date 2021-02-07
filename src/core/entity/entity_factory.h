#pragma once

#include <QtCore/QObject>

class Entity;
class EntityConfig;
class FactoryResolver;

class EntityFactory {
public:
    EntityFactory(const QString& id) : m_id(id) {}

    virtual Entity* createEntity(const EntityConfig& config, FactoryResolver* resolver) = 0;

    const QString& id() const {
        return m_id;
    }

protected:
    static QString requireString(const EntityConfig& config, const QString& key);
    static QString requireStringOr(const EntityConfig& config, const QString& key, const QString& defaultValue);
    static qint64 requireInt(const EntityConfig& config, const QString& key);
    static qint64 requireIntOr(const EntityConfig& config, const QString& key, qint64 defaultValue);
    static QVariantList requireList(const EntityConfig& config, const QString& key);
    static QVariantList requireListOr(const EntityConfig& config, const QString& key, const QVariantList& defaultValue);

    static QString requirePath(const EntityConfig& config, FactoryResolver* resolver, const QString& key);

private:
    template<class T>
    static T requireDataInternal(const EntityConfig& config, const QString& key, const T* defaultValue = nullptr);

private:
    QString m_id;
};
