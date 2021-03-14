#include "tray_icon_factory.h"

#include <core/setting/setting.h>
#include <core/skin/skin.h>

#include "tray_icon.h"

TrayIconFactory::TrayIconFactory() :
    EntityFactory(lit("tray_icon"))
{}

Entity* TrayIconFactory::createEntity(const EntityCreationContext& ctx) {
    Setting* setting = ctx.require<Setting*>(lit("target"));
    Skin* skin = ctx.require<Skin*>(lit("skin"));

    TrayIcon* result = new TrayIcon(ctx.id());
    result->setSkin(skin);
    result->setSetting(setting);
    return result;
}
