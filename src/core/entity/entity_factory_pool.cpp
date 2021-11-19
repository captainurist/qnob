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

std::unique_ptr<Entity> EntityFactoryPool::createEntity(const QString& typeId, QObject* parent) const {
    const EntityFactoryFunction* factoryFunction = value_ptr(m_factoryFunctionByTypeId, typeId);
    return factoryFunction ? (*factoryFunction)(parent) : nullptr;
}

