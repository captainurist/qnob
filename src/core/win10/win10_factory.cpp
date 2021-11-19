#include "win10_factory.h"

#include "win10.h"

Win10Factory::Win10Factory():
    EntityFactory(lit("win10"))
{}

Entity* Win10Factory::createEntity(const EntityCreationContext& ctx) {
    bool hideNativeOsd = ctx.requireOr<bool>(lit("hide_native_osd"), false);

    Win10* result = new Win10(ctx.id());

    if (hideNativeOsd) {
        result->ensureNativeOsdHidden();
        // TODO: also bind to settings changes!
    }

    return result;
}
