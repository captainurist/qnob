#pragma once

#include <lib/monitor/monitor_manager.h>

#include <core/entity/entity_pool.h>
#include <core/app/app.h>
#include <core/setting/brightness_setting_backend.h>
#include <core/setting/volume_setting_backend.h>
#include <core/setting/setting.h>
#include <core/tray_icon/standard_tray_icon.h>

class MonitorManager;

class DefaultEntityPool : public EntityPool {
public:
    DefaultEntityPool() {
        m_monitorManager.reset(new MonitorManager());

        Setting* volume = new Setting(lit("volume"), new VolumeSettingBackend());
        addEntity(volume);
        addEntity(new Setting(lit("brightness"), new BrightnessSettingBackend(m_monitorManager.get())));
        addEntity(new App(lit("app")));
        addEntity(new StandardTrayIcon(lit("volume_icon"), volume, AudioTrayIcon));
    }

    ~DefaultEntityPool() {
        clear();
    }

private:
    std::unique_ptr<MonitorManager> m_monitorManager;
};
