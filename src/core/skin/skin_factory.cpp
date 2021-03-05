#include "skin_factory.h"

#include <lib/serialization/serialization.h>

#include "horizontal_bar_skin.h"

SkinFactory::SkinFactory(SkinType type):
    EntityFactory(serialize(type)),
    m_type(type)
{}

Entity* SkinFactory::createEntity(const EntityCreationContext& ctx) {
    switch (m_type) {
    case HorizontalBarSkinType:
        return new HorizontalBarSkin(
            ctx.id(),
            ctx.require<QPixmap>(lit("background")),
            ctx.require<QPixmap>(lit("progress")),
            ctx.require<QPixmap>(lit("disabled")),
            QPoint(ctx.require<qint64>(lit("x")), ctx.require<qint64>(lit("y")))
        );
    default:
        assert(m_type == NumericSkinType);
        return nullptr;
    }

    // TODO: "units" not used right now
}
