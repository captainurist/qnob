#include "entity_pool_builder.h"

#include <QtCore/QScopeGuard>
#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>

#include <cassert>

#include <util/map_access.h>
#include <util/format.h>

#include "entity_factory_pool.h"
#include "entity_pool.h"
#include "entity_creation_exception.h"
#include "entity_creation_context.h"
#include "entity.h"

EntityPoolBuilder::EntityPoolBuilder(EntityFactoryPool* factoryPool, EntityPool* entityPool) {
    m_factoryPool = factoryPool;
    m_entityPool = entityPool;
}

void EntityPoolBuilder::addEntities(const QString& basePath, const VariantMap& records) {
    if (records.empty())
        return;

    auto forEachEntity = [&](auto function) {
        for (auto [id, record] : records) {
            if (id.isEmpty())
                continue; /* Should never happen really, but there are assertions down the line for this. */
            if (record.typeId() != MetaType::VariantMap)
                continue; /* Just ignore free-standing values for now. */
            function(EntityCreationContext(id, variantValueRef<VariantMap>(record), basePath, m_entityPool));
        }
    };

    /* Create all entities first. */
    forEachEntity([&](const EntityCreationContext& ctx) {
        if (m_entityPool->entity(ctx.id()))
            xthrow EntityCreationException(ctx.id(), sformat(EntityCreationException::tr("Entity '{}' already exists."), ctx.id()));

        QString type = ctx.require<QString>(lit("type"));
        const EntityFactoryFunction& factory = m_factoryPool->factory(type);
        if(!factory)
            xthrow EntityCreationException(ctx.id(), sformat(EntityCreationException::tr("Unknown entity type '{}'."), type));

        std::unique_ptr<Entity> entity = factory(nullptr);
        entity->initializeId(ctx.id());
        m_entityPool->addEntity(std::move(entity));
    });

    /* Then initialize them. */
    forEachEntity([&](const EntityCreationContext& ctx) {
        m_entityPool->entity(ctx.id())->load(ctx);
    });
}
