#include "monitor_enumerator.h"

#include <cassert>

#include <platform/platform.h>
#include <platform/platform_monitor_manager.h>
#include <util/debug.h>

#include "monitor.h"

MonitorEnumerator::MonitorEnumerator() {}

MonitorEnumerator::~MonitorEnumerator() {}

void MonitorEnumerator::enumerate() {
    assert(m_result.empty());

    for (auto& platformMonitor : platform()->monitorManager()->enumerateMonitors()) {
        std::unique_ptr<Monitor> monitor(new Monitor(std::move(platformMonitor)));
        QObject::connect(monitor.get(), &Monitor::initialized, this, &MonitorEnumerator::handleMonitorInitializated);
        m_result.emplace_back(std::move(monitor));
    }

    m_pendingInitializations = m_result.size();
    if (m_pendingInitializations == 0)
        QMetaObject::invokeMethod(this, [&] { emit finished(MonitorList()); }, Qt::QueuedConnection);

    xDebug("Enumerated {} monitor(s)", m_result.size());
    for (size_t i = 0; i < m_result.size(); i++)
        xDebug("Monitor #{}: {} (deviceId={})", i, m_result[i]->name(), m_result[i]->deviceId());
}

void MonitorEnumerator::handleMonitorInitializated() {
    m_pendingInitializations--;

    if (m_pendingInitializations == 0)
        emit finished(std::move(m_result));
}
