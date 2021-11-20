#include "entity.h"

#include <cassert>

Entity::Entity(QObject* parent) :
    QObject(parent)
{}

Entity::~Entity() {
    assert(!m_id.isEmpty()); /* initializeId was never called? */
}

void Entity::load(const EntityCreationContext& ctx) {
    loadFromConfig(ctx);
    m_loaded = true;
    emit loaded();
}

void Entity::loadFromConfig(const EntityCreationContext&) {
    /* Nothing to do here. */
}

void Entity::initializeId(const QString& id) {
    assert(m_id.isEmpty());
    assert(!id.isEmpty());

    m_id = id;
    setObjectName(id);
}
