#include "skin_factory.h"

#include <config/entity_config.h>
#include <core/entity/factory_resolver.h>
#include <core/entity/entity_creation_exception.h>

#include "skin.h"

SkinFactory::SkinFactory():
    EntityFactory(lit("skin"))
{}

Entity* SkinFactory::createEntity(const EntityCreationContext& ctx) {
    SkinData data;
    data.background = requireImage(ctx, lit("background"));
    data.progress = requireImage(ctx, lit("progress"));
    data.disabled = requireImage(ctx, lit("disabled"));
    data.offset.setX(ctx.requireInt(lit("x")));
    data.offset.setY(ctx.requireInt(lit("y")));
    data.units = ctx.requireInt(lit("units"));

    return new Skin(ctx.id(), data);
}

QPixmap SkinFactory::requireImage(const EntityCreationContext& ctx, const QString& key) {
    QString path = ctx.requirePath(key);
    QPixmap result(path);
    if (result.isNull())
        qthrow EntityCreationException(ctx.id(), EntityCreationException::tr("File '%1' is not a valid image file.").arg(path));
    return result;
}
