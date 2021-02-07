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

    QObject::connect(knob, &Knob::touched, result, [=] {
        result->update(skin->picture({ knob->enabled(), knob->value() }));
    });

    return result;
}
