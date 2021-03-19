#include "entity_pool_builder.h"

#include <QtCore/QScopeGuard>
#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>

#include <cassert>

#include <config/full_config.h>
#include <util/map_access.h>

#include "entity_factory_pool.h"
#include "entity_factory.h"
#include "entity_pool.h"
#include "entity_creation_exception.h"
#include "entity.h"

EntityPoolBuilder::EntityPoolBuilder(EntityFactoryPool* factoryPool, EntityPool* entityPool) {
    m_factoryPool = factoryPool;
    m_entityPool = entityPool;
}

EntityPoolBuilder::~EntityPoolBuilder() {}

void EntityPoolBuilder::addEntities(const FullConfig& fullConfig) {
    if (fullConfig.records.empty())
        return;

    m_dir = QFileInfo(fullConfig.path).absolutePath();
    m_configs = fullConfig.records;
    m_idsInFlight.clear();
    m_idStack.clear();

    for (auto [id, config] : m_configs) {
        if(config.typeId() != MetaType::VariantMap)
            continue; /* Just ignore free-standing values for now. */

        if (m_entities.contains(id))
            continue; /* Already created. */

        initEntity(id);
    }
}

void EntityPoolBuilder::initEntity(const QString& id) {
    assert(m_configs.contains(id));
    assert(m_configs[id].typeId() == MetaType::VariantMap);

    const VariantMap& config = variantValueRef<VariantMap>(m_configs[id]);

    QVariant typeVariant = value_or(config, lit("type"), QVariant());
    if (typeVariant.isNull())
        qthrow EntityCreationException(id, EntityCreationException::tr("Type not specified."));

    QString type = typeVariant.toString();
    EntityFactory* factory = m_factoryPool->factory(type);
    if (!factory)
        qthrow EntityCreationException(id, EntityCreationException::tr("Unknown entity type '%1'.").arg(type));

    m_idStack.push_back(id);
    m_idsInFlight.insert(id);
    auto cleanup = QScopeGuard([&] {
        m_idsInFlight.erase(id);
        m_idStack.pop_back();
    });

    std::unique_ptr<Entity> entity;
    try {
        entity.reset(factory->createEntity({ id, config, this }));
    } catch (EntityCreationException& e) {
        throw;
    } catch (...) {
        /* Wrap unknown exceptions in an EntityCreationException. */
        qthrow EntityCreationException(id, EntityCreationException::tr("Factory function failed."));
    }
    m_entities[id] = std::move(entity);
}

Entity* EntityPoolBuilder::resolveEntity(const QString& id) {
    if (m_idsInFlight.contains(id))
        qthrow EntityCreationException(m_idStack.back(), EntityCreationException::tr("Cyclical dependency detected."));

    if (Entity* result = m_entityPool->entity(id))
        return result;

    if (m_entities.contains(id))
        return m_entities[id].get();

    if (value_or(m_configs, id, QVariant()).typeId() != MetaType::VariantMap) {
        qthrow EntityCreationException(
            m_idStack.back(),
            EntityCreationException::tr("Entity refers to an unknown entity '%1'.").arg(id)
        );
    }

    initEntity(id);
    return m_entities[id].get();
}

QString EntityPoolBuilder::resolvePath(const QString& path) {
    QFileInfo info(path);
    if (info.isAbsolute())
        return path;

    return m_dir + QLatin1Char('/') + path;
}
