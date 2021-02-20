#include "win_monitor_manager.h"

#include "win_ddc_monitor.h"
#include "win_monitor.h"

#include <QtCore/QThread>

WinMonitorManager::WinMonitorManager() {}

std::vector<std::unique_ptr<PlatformMonitor>> WinMonitorManager::enumerateMonitors() {
    std::vector<std::unique_ptr<PlatformMonitor>> result;
    for (std::unique_ptr<WinDdcMonitor>& monitor : WinDdcMonitor::enumerateMonitors())
        result.emplace_back(new WinMonitor(std::move(monitor)));
    return result;
}
