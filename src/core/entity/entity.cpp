#include "entity.h"

#include <cassert>

Entity::Entity(QObject* parent) :
    QObject(parent)
{}

Entity::~Entity() {
    assert(!m_id.isEmpty()); /* initializeId was never called? */
}

void Entity::load(const EntityConfig& cfg) {
    loadFromConfig(cfg);
    m_loaded = true;
    emit loaded();
}

void Entity::loadFromConfig(const EntityConfig&) {
    /* Nothing to do here. */
}

void Entity::initializeId(const QString& id) {
    assert(m_id.isEmpty());
    assert(!id.isEmpty());

    m_id = id;
    setObjectName(id);
}
