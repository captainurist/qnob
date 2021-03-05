#include "tray_icon_factory.h"

#include <core/knob/knob.h>
#include <core/skin/skin.h>

#include "tray_icon.h"

TrayIconFactory::TrayIconFactory() :
    EntityFactory(lit("tray_icon"))
{}

Entity* TrayIconFactory::createEntity(const EntityCreationContext& ctx) {
    Knob* knob = ctx.require<Knob*>(lit("target"));
    Skin* skin = ctx.require<Skin*>(lit("skin"));

    TrayIcon* result = new TrayIcon(ctx.id());
    result->setSkin(skin);

    QObject::connect(knob, &Knob::activated, result, [=] {
        result->setState(knob->state());
    });

    return result;
}
