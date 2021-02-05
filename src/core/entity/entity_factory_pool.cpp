#include "entity_factory_pool.h"

#include <cassert>

#include <util/map_access.h>

#include "entity_factory.h"

EntityFactoryPool::~EntityFactoryPool() {}

void EntityFactoryPool::registerFactory(EntityFactory* factory) {
    assert(!m_factoryById.contains(factory->id()));

    m_factoryById[factory->id()].reset(factory);
}

EntityFactory* EntityFactoryPool::factory(const QString& id) const {
    return value_ref(m_factoryById, id).get();
}
