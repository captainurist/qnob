#pragma once

#include <memory>

#include <platform/platform_monitor_manager.h>

class WinMonitorManager : public PlatformMonitorManager {
    Q_OBJECT
public:
    WinMonitorManager();

    virtual std::vector<std::unique_ptr<PlatformMonitor>> enumerateMonitors() override;

private:
    std::shared_ptr<QThread> m_sharedThread;
};
