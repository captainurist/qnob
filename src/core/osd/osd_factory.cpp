#include "osd_factory.h"

#include <core/knob/knob.h>
#include <core/skin/skin.h>

#include <core/knob/knob.h>
#include <core/knob/knob_state.h>
#include <core/skin/skin.h>

#include <serialization/alignment.h>
#include <util/deferred_picture.h>

#include "osd.h"

OsdFactory::OsdFactory():
    EntityFactory(lit("osd"))
{}

Entity* OsdFactory::createEntity(const EntityCreationContext& ctx) {
    Knob* knob = ctx.requireEntity<Knob>(lit("target"));
    Skin* skin = ctx.requireEntity<Skin>(lit("skin"));

    Qt::Alignment alignment = ctx.requireSerializableOr<Qt::Alignment>(lit("position"), Qt::AlignBottom);
    int offsetX = ctx.requireIntOr(lit("offset_x"), 0);
    int offsetY = ctx.requireIntOr(lit("offset_y"), 0);

    Osd* result = new Osd(ctx.id());
    result->resize(skin->size());
    result->setAlignment(alignment);
    result->setOffset({ offsetX, offsetY });

    QObject::connect(knob, &Knob::activated, result, [=] {
        result->update(skin->picture(knob->state()));
        result->show(500, 500);
    });

    return result;
}