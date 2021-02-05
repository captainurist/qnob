#pragma once

#include <QtCore/QObject>

class Entity;
class EntityConfig;
class EntityResolver;

class EntityFactory: public QObject {
    Q_OBJECT
public:
    virtual Entity* createEntity(const EntityConfig& config, EntityResolver* resolver) = 0;
};
