#include "osd_factory.h"

#include <core/knob/knob.h>
#include <core/skin/skin.h>

#include <core/knob/knob.h>
#include <core/knob/knob_state.h>
#include <core/skin/skin.h>

#include <lib/serialization/alignment.h>

#include "osd.h"

OsdFactory::OsdFactory():
    EntityFactory(lit("osd"))
{}

Entity* OsdFactory::createEntity(const EntityCreationContext& ctx) {
    Knob* knob = ctx.require<Knob*>(lit("target"));
    Skin* skin = ctx.require<Skin*>(lit("skin"));

    Qt::Alignment alignment = ctx.requireOr<Qt::Alignment>(lit("position"), Qt::AlignBottom);
    int offsetX = ctx.requireOr<qint64>(lit("offset_x"), 0);
    int offsetY = ctx.requireOr<qint64>(lit("offset_y"), 0);

    Osd* result = new Osd(ctx.id());
    result->resize(skin->size());
    result->setPainter(skin->createPainter());
    result->setAlignment(alignment);
    result->setOffset({ offsetX, offsetY });

    QObject::connect(knob, &Knob::activated, result, [=] {
        result->setState(knob->state());
        result->show(500, 500);
    });

    return result;
}
