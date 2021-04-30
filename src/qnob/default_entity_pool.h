#pragma once

#include <core/entity/entity_pool.h>
#include <core/app/app.h>
#include <core/setting/brightness_setting_backend.h>
#include <core/setting/volume_setting_backend.h>
#include <core/setting/setting.h>
#include <core/tray_icon/standard_tray_icon.h>

class DefaultEntityPool : public EntityPool {
public:
    DefaultEntityPool() {
        Setting* volume = new Setting(lit("volume"), new VolumeSettingBackend());
        addEntity(volume);
        addEntity(new Setting(lit("brightness"), new BrightnessSettingBackend()));
        addEntity(new App(lit("app")));
        addEntity(new StandardTrayIcon(lit("volume_icon"), volume, AudioTrayIcon));
    }
};
