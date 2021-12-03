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
    Q_OBJECT
public:
    DefaultEntityPool(QObject* parent) : EntityPool(parent) {
        m_monitorManager.reset(new MonitorManager(this));
        m_upnpEndpoint.reset(new UpnpDiscoveryEndpoint(this));
        m_upnpEndpoint->start();

        std::unique_ptr<Setting> volume = std::make_unique<Setting>(std::make_unique<VolumeSettingBackend>(nullptr), this);
        addUninitializedEntity(lit("brightness"), std::make_unique<Setting>(std::make_unique<BrightnessSettingBackend>(m_monitorManager.get(), nullptr), this));
        addUninitializedEntity(lit("app"), std::make_unique<App>(this));
        addUninitializedEntity(lit("volume_icon"), std::make_unique<StandardTrayIcon>(AudioTrayIconControl, volume.get(), this));
        addUninitializedEntity(lit("heos"), std::make_unique<Heos>(m_upnpEndpoint.get(), this));
        addUninitializedEntity(lit("volume"), std::move(volume));
    }

    ~DefaultEntityPool() {
        clear();
    }

private:
    void addUninitializedEntity(const QString& id, std::unique_ptr<Entity> entity) {
        entity->initializeId(id);
        addEntity(std::move(entity));
    }

private:
    std::unique_ptr<MonitorManager> m_monitorManager;
    std::unique_ptr<UpnpDiscoveryEndpoint> m_upnpEndpoint;
};
