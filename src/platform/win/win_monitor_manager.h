#pragma once

#include <memory>

#include <platform/platform_monitor_manager.h>

class WinSharedEventWindow;

class WinMonitorManager : public PlatformMonitorManager {
    Q_OBJECT
public:
    WinMonitorManager(WinSharedEventWindow* eventWindow);
    virtual ~WinMonitorManager();

    virtual std::vector<std::unique_ptr<PlatformMonitor>> enumerateMonitors() override;
};
