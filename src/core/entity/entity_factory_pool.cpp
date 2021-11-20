#include "entity_factory_pool.h"

#include <cassert>

#include <util/map_access.h>

#include "entity.h"

EntityFactoryPool::EntityFactoryPool(QObject* parent) :
    QObject(parent)
{}

EntityFactoryPool::~EntityFactoryPool() {}

void EntityFactoryPool::registerFactory(const QString& typeId, const EntityFactoryFunction& factoryFunction) {
    assert(!m_factoryFunctionByTypeId.contains(typeId));

    m_factoryFunctionByTypeId.emplace(typeId, factoryFunction);
}

const EntityFactoryFunction& EntityFactoryPool::factory(const QString& typeId) const {
    return value_ref(m_factoryFunctionByTypeId, typeId, m_emptyFactory);
}
