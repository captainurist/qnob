#include "entity_pool.h"

#include <cassert>

#include <util/map_access.h>

#include "entity.h"
#include "entity_factory_pool.h"
#include "entity_creation_exception.h"
#include "entity_creation_context.h"

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

void EntityPool::loadEntities(const QString& basePath, const EntityFactoryPool* factoryPool, const std::unordered_map<QString, VariantMap>& configs) {
    if (configs.empty())
        return;

    if (configs.contains(QString()))
        xthrow EntityCreationException(QString(), EntityCreationException::tr("Empty string is not a valid entity id."));

    for (auto&& [id, config] : configs)
        if (entity(id))
            xthrow EntityCreationException(id, sformat(EntityCreationException::tr("Entity '{}' already exists."), id));

    try {
        /* Create all entities first. */
        for (auto&& [id, config] : configs) {
            EntityCreationContext ctx(id, config, basePath, this);

            QString type = ctx.require<QString>(lit("type"));
            const EntityFactoryFunction& factory = factoryPool->factory(type);
            if (!factory)
                xthrow EntityCreationException(ctx.id(), sformat(EntityCreationException::tr("Unknown entity type '{}'."), type));

            std::unique_ptr<Entity> entity = factory(this);
            entity->initializeId(id);
            addEntity(std::move(entity));
        }

        /* Then load them. */
        for (auto&& [id, config] : configs)
            entity(id)->load(EntityCreationContext(id, config, basePath, this));
    } catch (...) {
        /* Clean up whatever we've added to this pool. */
        for (auto&& [id, config] : configs)
            m_entityById.erase(id);
        throw;
    }
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
