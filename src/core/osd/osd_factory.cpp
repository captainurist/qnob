#include "osd_factory.h"

#include <core/knob/knob.h>
#include <core/skin/skin.h>

#include <core/knob/knob.h>
#include <core/knob/knob_state.h>
#include <core/skin/skin.h>

#include <util/deferred_picture.h>

#include "osd.h"

OsdFactory::OsdFactory():
    EntityFactory(lit("osd"))
{}

Entity* OsdFactory::createEntity(const EntityCreationContext& ctx) {
    Knob* knob = ctx.requireEntity<Knob>(lit("target"));
    Skin* skin = ctx.requireEntity<Skin>(lit("skin"));

    Osd* result = new Osd(ctx.id());
    result->resize(skin->size());
    result->setAlignment(Qt::AlignBottom);
    result->setOffset({ 0, -100 });

    QObject::connect(knob, &Knob::activated, result, [=] {
        result->update(skin->picture(knob->state()));
        result->show(500, 500);
    });

    return result;
}
