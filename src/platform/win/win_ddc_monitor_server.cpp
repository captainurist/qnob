#include "win_ddc_monitor_server.h"

#include <cassert>

#include "win_ddc_monitor.h"
#include "win_ddc_monitor_queue.h"

WinDdcMonitorServer::WinDdcMonitorServer(std::unique_ptr<WinDdcMonitor> monitor, std::shared_ptr<WinDdcMonitorQueue> queue) :
    m_monitor(std::move(monitor)),
    m_queue(queue)
{}

WinDdcMonitorServer::~WinDdcMonitorServer() {}

void WinDdcMonitorServer::processQueue() {
    WinDdcMonitorAction action;
    while (m_queue->popAction(&action)) {
        if (action.type == WinDdcMonitorAction::Read) {
            WinDdcTriplet value;
            bool ok = m_monitor->read(action.property, &value);
            emit readCompleted(ok, action.property, value);
        } else {
            assert(action.type == WinDdcMonitorAction::Write);
            bool ok = m_monitor->write(action.property, action.value);
            emit writeCompleted(ok, action.property, action.value);
        }
    }
}

