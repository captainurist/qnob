#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <QtCore/QVariant>
#include <QtCore/QHashFunctions>

#include <util/variant.h>

#include "factory_resolver.h"

class EntityPool;
class EntityFactoryPool;
class EntityConfig;

class EntityPoolBuilder: private FactoryResolver {
public:
    EntityPoolBuilder(EntityFactoryPool* factoryPool, EntityPool* entityPool);
    ~EntityPoolBuilder();

    void addEntities(const QString& basePath, const VariantMap& records);

private:
    void initEntity(const QString& id);
    virtual Entity* resolveEntity(const QString& id) override;
    virtual QString resolvePath(const QString& path) override;

private:
    EntityFactoryPool* m_factoryPool;
    EntityPool* m_entityPool;
    QString m_dir;
    VariantMap m_configs;
    std::unordered_map<QString, std::unique_ptr<Entity>> m_entities;
    std::unordered_set<QString> m_idsInFlight;
    std::vector<QString> m_idStack;
};

