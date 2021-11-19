#pragma once

#include <core/entity/entity_factory_pool.h>
#include <core/knob/knob_factory.h>
#include <core/hotkey/hotkey_factory.h>
#include <core/sound/sound_factory.h>
#include <core/skin/skin_factory.h>
#include <core/osd/osd_factory.h>
#include <core/tray_icon/tray_icon_factory.h>
#include <core/tray_event/tray_event_factory.h>
#include <core/win10/win10_factory.h>

class DefaultEntityFactoryPool : public EntityFactoryPool {
public:
    DefaultEntityFactoryPool() {
        registerFactory(std::make_unique<KnobFactory>());
        registerFactory(std::make_unique<HotkeyFactory>());
        registerFactory(std::make_unique<SoundFactory>());
        registerFactory(std::make_unique<SkinFactory>(HorizontalBarSkinType));
        registerFactory(std::make_unique<SkinFactory>(NumericSkinType));
        registerFactory(std::make_unique<OsdFactory>());
        registerFactory(std::make_unique<TrayIconFactory>());
        registerFactory(std::make_unique<TrayEventFactory>());
        registerFactory(std::make_unique<Win10Factory>());
    }
};
