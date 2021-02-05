#include "entity_pool_builder.h"

#include <QtCore/QCoreApplication>

#include <cassert>

#include <config/full_config.h>
#include <utility/map_access.h>

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

    m_configs = fullConfig.records;
    m_entities.resize(m_configs.size());

    m_indexById.clear();
    for (size_t i = 0; i < m_configs.size(); i++)
        m_indexById[m_configs[i].id] = i;
    assert(m_indexById.size() == m_configs.size()); /* Expect unique ids. */

    m_idsInFlight.clear();
    m_idStack.clear();

    for (size_t i = 0; i < m_configs.size(); i++) {
        if(m_entities[i])
            continue; /* Already created. */

        createEntity(i);
    }
}

void EntityPoolBuilder::createEntity(size_t index) {
    const EntityConfig& config = m_configs[index];

    EntityFactory* factory = m_factoryPool->factory(config.type);
    if (!factory)
        qthrow EntityCreationException(config.id, EntityCreationException::tr("Unknown entity type '%1'.").arg(config.type));

    m_idStack.push_back(config.id);
    m_idsInFlight.insert(config.id);
    m_entities[index].reset(factory->createEntity(config, this));
    m_idsInFlight.erase(config.id);
    m_idStack.pop_back();
}

Entity* EntityPoolBuilder::resolveEntity(const QString& id) {
    if (m_idsInFlight.contains(id))
        qthrow EntityCreationException(m_idStack.back(), EntityCreationException::tr("Cyclical dependency detected."));

    if (Entity* result = m_entityPool->entity(id))
        return result;

    int index = value(m_indexById, id, -1);
    if (index == -1) {
        qthrow EntityCreationException(
            m_idStack.back(),
            EntityCreationException::tr("Entity refers to an unknown entity '%1'.").arg(id)
        );
    }

    if (!m_entities[index])
        createEntity(index);

    assert(m_entities[index]);
    return m_entities[index].get();
}
