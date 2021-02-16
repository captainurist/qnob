#include "skin_factory.h"

#include <config/entity_config.h>
#include <core/entity/factory_resolver.h>
#include <core/entity/entity_creation_exception.h>

#include "skin.h"
#include "skin_data.h"

SkinFactory::SkinFactory():
    EntityFactory(lit("skin"))
{}

Entity* SkinFactory::createEntity(const EntityCreationContext& ctx) {
    SkinData data;
    data.background = ctx.require<QPixmap>(lit("background"));
    data.progress = ctx.require<QPixmap>(lit("progress"));
    data.disabled = ctx.require<QPixmap>(lit("disabled"));
    data.offset.setX(ctx.require<qint64>(lit("x")));
    data.offset.setY(ctx.require<qint64>(lit("y")));
    data.units = ctx.require<qint64>(lit("units"));

    return new Skin(ctx.id(), data);
}
