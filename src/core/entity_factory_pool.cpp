#include "entity_factory_pool.h"

#include <cassert>

#include <utility/map_access.h>

#include "entity_factory.h"

EntityFactoryPool::~EntityFactoryPool() {}

void EntityFactoryPool::registerFactory(const QString& id, EntityFactory* factory) {
    assert(!m_factoryById.contains(id));
    m_factoryById[id].reset(factory);
}

EntityFactory* EntityFactoryPool::factory(const QString& id) const {
    return value_ref(m_factoryById, id).get();
}
