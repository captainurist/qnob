#include "win_ddc_monitor_client.h"

#include "win_ddc_monitor_queue.h"

WinDdcMonitorClient::WinDdcMonitorClient(std::shared_ptr<WinDdcMonitorQueue> queue) :
    m_queue(queue)
{}

void WinDdcMonitorClient::read(PlatformMonitor::Property property) {
    m_queue->addAction({ property, WinDdcMonitorAction::Read, 0 });
    emit queueChanged();
}

void WinDdcMonitorClient::write(PlatformMonitor::Property property, DWORD value) {
    m_queue->addAction({ property, WinDdcMonitorAction::Write, value });
    emit queueChanged();
}
