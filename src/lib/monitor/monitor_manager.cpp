#include "monitor_manager.h"

#include <platform/platform.h>
#include <platform/platform_monitor_manager.h>

#include "monitor.h"
#include "monitor_enumerator.h"

MonitorManager::MonitorManager(QObject* parent): 
    QObject(parent) 
{
    connect(platform()->monitorManager(), &PlatformMonitorManager::monitorsChanged, this, &MonitorManager::updateMonitors);

    updateMonitors();
}

MonitorManager::~MonitorManager() {}

const MonitorList& MonitorManager::monitors() const {
    return m_monitors;
}

void MonitorManager::updateMonitors() {
    MonitorEnumerator* enumerator = new MonitorEnumerator(this); /* Destroy enumerator if we get destroyed before it's finished. */
    connect(enumerator, &MonitorEnumerator::finished, this, &MonitorManager::handleEnumeratorFinished);
    connect(enumerator, &MonitorEnumerator::finished, enumerator, &QObject::deleteLater);
    enumerator->enumerate();
}

void MonitorManager::handleEnumeratorFinished(Package<MonitorList> monitors) {
    m_monitors = monitors.take();
    emit monitorsChanged();
}

