#include "knob_factory.h"

#include <core/setting/setting.h>

#include "knob.h"

KnobFactory::KnobFactory() :
    EntityFactory(lit("knob"))
{}

Entity* KnobFactory::createEntity(const EntityCreationContext& ctx) {
    std::unique_ptr<Knob> result(new Knob(ctx.id()));
    result->setStep(ctx.require<double>(lit("step")));
    result->setSetting(ctx.require<Setting*>(lit("target")));
    return result.release();
}
