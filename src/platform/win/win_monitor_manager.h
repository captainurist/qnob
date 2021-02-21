#pragma once

#include <memory>

#include <platform/platform_monitor_manager.h>


// TODO: WM_DISPLAYCHANGE is sent when # of monitors is changed.

class WinMonitorManager : public PlatformMonitorManager {
    Q_OBJECT
public:
    WinMonitorManager();

    virtual std::vector<std::unique_ptr<PlatformMonitor>> enumerateMonitors() override;
};
