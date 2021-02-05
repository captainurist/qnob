#pragma once

#include <QtCore/QObject>

class Entity;
class EntityConfig;
class EntityResolver;

class EntityFactory {
public:
    EntityFactory(const QString& id) : m_id(id) {}

    virtual Entity* createEntity(const EntityConfig& config, EntityResolver* resolver) = 0;

    const QString& id() const {
        return m_id;
    }

protected:
    template<class T>
    static T requireData(const EntityConfig& config, const QString& key) {
        return requireDataInternal<T>(config, key);
    }

    template<class T>
    static T requireDataOr(const EntityConfig& config, const QString& key, const T& defaultValue) {
        return requireDataInternal<T>(config, key, &defaultValue);
    }

private:
    template<class T>
    static T requireDataInternal(const EntityConfig& config, const QString& key, const T* defaultValue = nullptr);

private:
    QString m_id;
};
