#include "entity_pool.h"

#include <cassert>

#include <util/map_access.h>

#include "entity.h"

EntityPool::EntityPool(QObject* parent) :
    QObject(parent)
{}

EntityPool::~EntityPool() {}

void EntityPool::addEntity(std::unique_ptr<Entity> entity) {
    assert(entity);
    assert(!entity->id().isEmpty()); /* Forgot to call Entity::initializeId? */
    assert(!m_entityById.contains(entity->id()));

    entity->setParent(this);
    m_entityById.emplace(entity->id(), std::move(entity));
}

Entity* EntityPool::entity(const QString& id) const {
    return value_ref(m_entityById, id).get();
}

std::vector<Entity*> EntityPool::entities() const {
    std::vector<Entity*> result;
    for (const auto& [id, entity] : m_entityById)
        result.push_back(entity.get());
    return result;
}

void EntityPool::clear() {
    m_entityById.clear();
}
