#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <QtCore/QHashFunctions>
#include <QtCore/QByteArray>

#include "factory_resolver.h"

class EntityPool;
class EntityFactoryPool;
class EntityConfig;
class FullConfig;

class EntityPoolBuilder: private FactoryResolver {
public:
    EntityPoolBuilder(EntityFactoryPool* factoryPool, EntityPool* entityPool);
    ~EntityPoolBuilder();

    void addEntities(const FullConfig& fullConfig);

private:
    void createEntity(size_t index);
    virtual Entity* resolveEntity(const QString& id) override;
    virtual QString resolvePath(const QString& path) override;

private:
    EntityFactoryPool* m_factoryPool;
    EntityPool* m_entityPool;
    QString m_dir;
    std::vector<EntityConfig> m_configs;
    std::vector<std::unique_ptr<Entity>> m_entities;
    std::unordered_map<QString, int> m_indexById;
    std::unordered_set<QString> m_idsInFlight;
    std::vector<QString> m_idStack;
};

