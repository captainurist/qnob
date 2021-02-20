#include "win_monitor_manager.h"

#include "win_ddc_monitor.h"
#include "win_monitor.h"

#include <QtCore/QThread>

WinMonitorManager::WinMonitorManager() {
    m_sharedThread.reset(new QThread(), [](QThread* thread) {
        thread->quit();
        thread->wait();
        thread->deleteLater();
    });
    m_sharedThread->start();
}

std::vector<std::unique_ptr<PlatformMonitor>> WinMonitorManager::enumerateMonitors() {
    std::vector<std::unique_ptr<PlatformMonitor>> result;
    for (std::unique_ptr<WinDdcMonitor>& monitor : WinDdcMonitor::enumerateMonitors())
        result.emplace_back(new WinMonitor(m_sharedThread, std::move(monitor)));
    return result;
}
