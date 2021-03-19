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
    QPoint offset = ctx.requireOr<QPoint>(lit("offset"), QPoint(0, 0));

    Osd* result = new Osd(ctx.id());
    result->setAlignment(alignment);
    result->setOffset(offset);
    result->setSkin(skin);
    result->setSetting(setting);
    return result;
}
