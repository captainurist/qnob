#pragma once

#include <lib/monitor/monitor_manager.h>
#include <lib/upnp/upnp_discovery_endpoint.h>

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
        m_monitorManager.reset(new MonitorManager(nullptr));
        m_upnpEndpoint.reset(new UpnpDiscoveryEndpoint(nullptr));
        m_upnpEndpoint->start();

        std::unique_ptr<Setting> volume = std::make_unique<Setting>(lit("volume"), std::make_unique<VolumeSettingBackend>());
        addEntity(std::make_unique<Setting>(lit("brightness"), std::make_unique<BrightnessSettingBackend>(m_monitorManager.get())));
        addEntity(std::make_unique<App>(lit("app")));
        addEntity(std::make_unique<StandardTrayIcon>(lit("volume_icon"), volume.get(), AudioTrayIcon));
        addEntity(std::make_unique<Heos>(lit("heos"), m_upnpEndpoint.get()));
        addEntity(std::move(volume));

    }

    ~DefaultEntityPool() {
        clear();
    }

private:
    std::unique_ptr<MonitorManager> m_monitorManager;
    std::unique_ptr<UpnpDiscoveryEndpoint> m_upnpEndpoint;
};
