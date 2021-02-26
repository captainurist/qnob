#pragma once

#include <memory>

#include <platform/platform_monitor_manager.h>

class WinNativeEventWindow;

class WinMonitorManager : public PlatformMonitorManager {
    Q_OBJECT
public:
    WinMonitorManager();
    virtual ~WinMonitorManager();

    virtual std::vector<std::unique_ptr<PlatformMonitor>> enumerateMonitors() override;

private:
    void dispatchEvent(void* message);

private:
    std::unique_ptr<WinNativeEventWindow> m_eventWindow;
};
