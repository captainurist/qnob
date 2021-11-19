#pragma once

#include <QtCore/QString>

#include <util/variant.h>

class EntityPool;
class EntityFactoryPool;

// TODO: drop this class?
class EntityPoolBuilder {
public:
    EntityPoolBuilder(EntityFactoryPool* factoryPool, EntityPool* entityPool);

    void addEntities(const QString& basePath, const VariantMap& records);

private:
    EntityFactoryPool* m_factoryPool;
    EntityPool* m_entityPool;
};

