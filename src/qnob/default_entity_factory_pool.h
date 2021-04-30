#pragma once

#include <core/entity/entity_factory_pool.h>
#include <core/knob/knob_factory.h>
#include <core/hotkey/hotkey_factory.h>
#include <core/sound/sound_factory.h>
#include <core/skin/skin_factory.h>
#include <core/osd/osd_factory.h>
#include <core/tray_icon/tray_icon_factory.h>
#include <core/tray_event/tray_event_factory.h>

class DefaultEntityFactoryPool : public EntityFactoryPool {
public:
    DefaultEntityFactoryPool() {
        registerFactory(new KnobFactory());
        registerFactory(new HotkeyFactory());
        registerFactory(new SoundFactory());
        registerFactory(new SkinFactory(HorizontalBarSkinType));
        registerFactory(new SkinFactory(NumericSkinType));
        registerFactory(new OsdFactory());
        registerFactory(new TrayIconFactory());
        registerFactory(new TrayEventFactory());
    }
};
