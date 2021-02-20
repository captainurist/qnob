#include "win_monitor.h"

#include <highlevelmonitorconfigurationapi.h>

#include <QtCore/QThread>

#include <util/debug.h>

#include "win_ddc_monitor.h"
#include "win_ddc_monitor_queue.h"
#include "win_ddc_monitor_client.h"
#include "win_ddc_monitor_server.h"

WinMonitor::WinMonitor(std::unique_ptr<WinDdcMonitor> monitor) {
    /* These call doesn't take forever to complete, so we do them in main thread, and cache the results. */
    DWORD colorTemperatures;
    monitor->readCapabilities(&m_capabilities, &colorTemperatures);
    m_name = monitor->name();

    /* Then set up a processing thread. */
    m_thread.reset(new QThread());
    m_thread->start();

    /* Then we can do the actual client-server initialization. */
    std::shared_ptr<WinDdcMonitorQueue> queue = std::make_shared<WinDdcMonitorQueue>();
    m_client.reset(new WinDdcMonitorClient(queue));
    m_server = new WinDdcMonitorServer(std::move(monitor), queue);
    m_server->moveToThread(m_thread.get());

    connect(m_client.get(), &WinDdcMonitorClient::queueChanged, m_server, &WinDdcMonitorServer::processQueue);
    connect(m_server, &WinDdcMonitorServer::readCompleted, this, &WinMonitor::handleReadCompleted);
    connect(m_server, &WinDdcMonitorServer::writeCompleted, this, &WinMonitor::handleWriteCompleted);

    /* And finally request property values. */
    updateProperty(BrightnessProperty);
    updateProperty(ContrastProperty);
}

WinMonitor::~WinMonitor() {
    /* Fire deleteLater for the server. We'll then be stopping the thread, but deleteLater event will
     * already be in the thread's event queue at this point, and will be processed. */
    m_server->deleteLater();

    /* Clean up the thread. */
    m_thread->quit();
    m_thread->wait(); /* This is where m_server gets deleted. */
}

QString WinMonitor::name() const {
    return m_name;
}

bool WinMonitor::supported(Property property) const {
    switch (property) {
    case PlatformMonitor::BrightnessProperty:
        return m_capabilities & MC_CAPS_BRIGHTNESS;
    case PlatformMonitor::ContrastProperty:
        return m_capabilities & MC_CAPS_CONTRAST;
    default:
        QB_VERIFY(false);
    }
}

float WinMonitor::property(Property property) const {
    return mapFromNative(m_properties[property].triplet);
}

void WinMonitor::setProperty(Property property, float value) {
    if (!supported(property))
        return;

    PropertyState& state = m_properties[property];
    if (state.triplet == WinDdcTriplet()) {
        state.pendingWrite = value;
        return;
    }

    m_client->write(property, mapToNative(state.triplet, value));
}

void WinMonitor::updateProperty(Property property) {
    if(supported(property))
        m_client->read(property);
}

void WinMonitor::handleReadCompleted(bool ok, PlatformMonitor::Property property, WinDdcTriplet value) {
    if (!ok)
        return;

    PropertyState& state = m_properties[property];
    state.triplet = value;

    if (!std::isnan(state.pendingWrite)) {
        m_client->write(property, mapToNative(state.triplet, state.pendingWrite));
        state.pendingWrite = NAN;
    }
}

void WinMonitor::handleWriteCompleted(bool ok, PlatformMonitor::Property property, DWORD value) {
    if (!ok)
        return;

    PropertyState& state = m_properties[property];
    assert(state.triplet != WinDdcTriplet());

    state.triplet.current = value;

    emit propertyChanged(property);
}

DWORD WinMonitor::mapToNative(const WinDdcTriplet& triplet, float value) {
    assert(triplet != WinDdcTriplet());

    return triplet.min + (triplet.max - triplet.min) * std::clamp(value, 0.0f, 1.0f);
}

float WinMonitor::mapFromNative(const WinDdcTriplet& triplet) {
    /* Just return NAN for uninitialized triplets. */
    return (triplet.current - triplet.min) * 1.0f / (triplet.max - triplet.min);
}
