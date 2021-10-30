#include "monitor.h"

#include <cassert>

#include <QtCore/QThread>

#include "monitor_queue.h"
#include "monitor_server.h"

Monitor::Monitor(std::unique_ptr<PlatformMonitor> monitor) {
    /* Getting a name & device id doesn't take forever to complete, so we do in in ctor. */
    m_cachedDeviceId = monitor->deviceId();
    m_cachedName = monitor->name();

    /* Set up a processing thread. */
    m_thread.reset(new QThread());
    m_thread->setObjectName(lit("MonitorServerThread"));
    m_thread->start();

    /* Initialize monitor server. */
    m_queue = std::make_shared<MonitorQueue>();
    monitor->moveToThread(m_thread.get());
    m_server = new MonitorServer(std::move(monitor), m_queue);
    m_server->moveToThread(m_thread.get());

    connect(this, &Monitor::notifyServer, m_server, &MonitorServer::processQueue);
    connect(m_server, &MonitorServer::capabilitiesCompleted, this, &Monitor::handleCapabilitiesCompleted);
    connect(m_server, &MonitorServer::readCompleted, this, &Monitor::handleReadCompleted);
    connect(m_server, &MonitorServer::writeCompleted, this, &Monitor::handleWriteCompleted);
    connect(m_thread.get(), &QThread::finished, m_server, &QObject::deleteLater);

    /* Fire initialization requests. */
    m_queue->addAction({ MonitorAction::ReadCapabilities });
    m_queue->addAction({ MonitorAction::ReadProperty, BrightnessMonitorProperty });
    m_queue->addAction({ MonitorAction::ReadProperty, ContrastMonitorProperty });
    m_pendingInitializationRequests = 3;
    emit notifyServer();
}

Monitor::~Monitor() {
    /* Clean up the thread. */
    m_thread->quit();
    m_thread->wait(); /* This is where m_server gets deleted. */
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
