#pragma once

#include <lib/monitor/monitor_manager.h>
#include <lib/upnp/upnp_discovery.h>

#include <core/entity/entity_pool.h>
#include <core/app/app.h>
#include <core/setting/brightness_setting_backend.h>
#include <core/setting/volume_setting_backend.h>
#include <core/setting/setting.h>
#include <core/tray_icon/standard_tray_icon.h>
#include <core/heos/heos.h>

class MonitorManager;

class DefaultEntityPool : public EntityPool {
public:
    DefaultEntityPool() {
        m_monitorManager.reset(new MonitorManager());
        m_upnpDiscovery.reset(new UpnpDiscovery(UpnpDiscoveryOptions()));

        Setting* volume = new Setting(lit("volume"), new VolumeSettingBackend());
        addEntity(volume);
        addEntity(new Setting(lit("brightness"), new BrightnessSettingBackend(m_monitorManager.get())));
        addEntity(new App(lit("app")));
        addEntity(new StandardTrayIcon(lit("volume_icon"), volume, AudioTrayIcon));
        addEntity(new Heos(lit("heos"), m_upnpDiscovery.get()));

        m_upnpDiscovery->start();
    }

    ~DefaultEntityPool() {
        clear();
    }

private:
    std::unique_ptr<MonitorManager> m_monitorManager;
    std::unique_ptr<UpnpDiscovery> m_upnpDiscovery;
};
