#include "osd_factory.h"

#include <lib/serializable_types/alignment.h>
#include <core/setting/setting.h>
#include <core/skin/skin.h>

#include "osd.h"

OsdFactory::OsdFactory():
    EntityFactory(lit("osd"))
{}

Entity* OsdFactory::createEntity(const EntityCreationContext& ctx) {
    Setting* setting = ctx.require<Setting*>(lit("target"));
    Skin* skin = ctx.require<Skin*>(lit("skin"));

    Qt::Alignment alignment = ctx.requireOr<Qt::Alignment>(lit("position"), Qt::AlignBottom);
    int offsetX = ctx.requireOr<qint64>(lit("offset_x"), 0);
    int offsetY = ctx.requireOr<qint64>(lit("offset_y"), 0); // TODO: array!

    Osd* result = new Osd(ctx.id());
    result->setAlignment(alignment);
    result->setOffset({ offsetX, offsetY });
    result->setSkin(skin);
    result->setSetting(setting);
    return result;
}
