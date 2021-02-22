#include "monitor_server.h"

#include <cassert>

#include "monitor.h"
#include "monitor_queue.h"

MonitorServer::MonitorServer(std::unique_ptr<PlatformMonitor> monitor, std::shared_ptr<MonitorQueue> queue) :
    m_monitor(std::move(monitor)),
    m_queue(queue)
{}

MonitorServer::~MonitorServer() {}

void MonitorServer::processQueue() {
    MonitorAction action;
    while (m_queue->popAction(&action)) {
        switch (action.type) {
        case MonitorAction::ReadCapabilities:
            emit capabilitiesCompleted(m_monitor->capabilities());
            break;
        case MonitorAction::ReadProperty:
            emit readCompleted(action.property, m_monitor->property(action.property));
            break;
        case MonitorAction::WriteProperty:
            m_monitor->setProperty(action.property, action.value);
            emit writeCompleted(action.property, action.value);
            break;
        default:
            assert(false);
        }
    }
}
