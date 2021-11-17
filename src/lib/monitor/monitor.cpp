#include "monitor.h"

#include <cassert>

#include <QtCore/QThread>

#include <util/thread/worker_pool.h>

#include "monitor_queue.h"
#include "monitor_server.h"

Monitor::Monitor(std::unique_ptr<PlatformMonitor> monitor, QObject* parent): PlatformMonitor(parent) {
    /* Getting a name & device id doesn't take forever to complete, so we do in in ctor. */
    m_cachedDeviceId = monitor->deviceId();
    m_cachedName = monitor->name();

    /* Initialize monitor server. */
    m_queue = std::make_shared<MonitorQueue>();
    std::unique_ptr<MonitorServer> server = std::make_unique<MonitorServer>(std::move(monitor), m_queue, nullptr);
    m_server = server.get();

    connect(this, &Monitor::notifyServer, m_server, &MonitorServer::processQueue);
    connect(m_server, &MonitorServer::capabilitiesCompleted, this, &Monitor::handleCapabilitiesCompleted);
    connect(m_server, &MonitorServer::readCompleted, this, &Monitor::handleReadCompleted);
    connect(m_server, &MonitorServer::writeCompleted, this, &Monitor::handleWriteCompleted);

    WorkerPool::globalInstance()->run(std::move(server));

    /* Fire initialization requests. */
    m_queue->addAction({ MonitorAction::ReadCapabilities });
    m_queue->addAction({ MonitorAction::ReadProperty, BrightnessMonitorProperty });
    m_queue->addAction({ MonitorAction::ReadProperty, ContrastMonitorProperty });
    m_pendingInitializationRequests = 3;
    emit notifyServer();
}

Monitor::~Monitor() {
    WorkerPool::globalInstance()->kill(m_server);

    /* We don't wait for the server to get destroyed. */
}

bool Monitor::isInitialized() const {
    return m_pendingInitializationRequests == 0;
}

QString Monitor::deviceId() const {
    return m_cachedDeviceId;
}

QString Monitor::name() const {
    return m_cachedName;
}

PlatformMonitorProperties Monitor::capabilities() const {
    assert(isInitialized());

    return m_cachedCapabilities;
}

float Monitor::property(PlatformMonitorProperty property) const {
    assert(isInitialized());

    return m_cachedProperties[property];
}

void Monitor::setProperty(PlatformMonitorProperty property, float value) {
    assert(isInitialized());

    m_queue->addAction({ MonitorAction::WriteProperty, property, value });
    emit notifyServer();
}

void Monitor::handleInitializationReply() {
    if (isInitialized())
        return;

    m_pendingInitializationRequests--;

    if(isInitialized())
        emit initialized();
}

void Monitor::handleCapabilitiesCompleted(PlatformMonitorProperties capabilities) {
    m_cachedCapabilities = capabilities;

    handleInitializationReply();
}

void Monitor::handleReadCompleted(PlatformMonitorProperty property, float value) {
    m_cachedProperties[property] = value;

    handleInitializationReply();
}

void Monitor::handleWriteCompleted(PlatformMonitorProperty property, float value) {
    m_cachedProperties[property] = value;
}
