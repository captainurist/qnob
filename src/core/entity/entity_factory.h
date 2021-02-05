#pragma once

#include <QtCore/QObject>

class Entity;
class EntityConfig;
class EntityResolver;

class EntityFactory: public QObject {
    Q_OBJECT
public:
    EntityFactory(const QString& id) : m_id(id) {}

    virtual Entity* createEntity(const EntityConfig& config, EntityResolver* resolver) = 0;

    const QString& id() const {
        return m_id;
    }

private:
    QString m_id;
};
