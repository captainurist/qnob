#include "entity_pool.h"

#include <cassert>

#include <utility/map_access.h>

#include "entity.h"

EntityPool::~EntityPool() {}

void EntityPool::addEntity(Entity* entity) {
    assert(!m_entityById.contains(entity->id()));
    m_entityById[entity->id()].reset(entity);
}

Entity* EntityPool::entity(const QString& id) const {
    return value_ref(m_entityById, id).get();
}
