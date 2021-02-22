#include "win_monitor.h"

#include <cmath> /* For NAN. */
#include <algorithm> /* For std::clamp. */

#include <highlevelmonitorconfigurationapi.h>

#include "win_ddc_monitor.h"

static DWORD mapToNative(const WinDdcTriplet& triplet, float value) {
    assert(triplet.min < triplet.max);

    return triplet.min + (triplet.max - triplet.min) * std::clamp(value, 0.0f, 1.0f);
}

static float mapFromNative(const WinDdcTriplet& triplet) {
    assert(triplet.min < triplet.max);

    return (triplet.current - triplet.min) * 1.0f / (triplet.max - triplet.min);
}

WinMonitor::WinMonitor(std::unique_ptr<WinDdcMonitor> monitor):
    m_monitor(std::move(monitor))
{}

WinMonitor::~WinMonitor() {}

QString WinMonitor::name() const {
    return m_monitor->name();
}

WinMonitor::Properties WinMonitor::capabilities() const {
    DWORD capabilities;
    m_monitor->readCapabilities(&capabilities);

    WinMonitor::Properties result;
    if (capabilities & MC_CAPS_BRIGHTNESS)
        result |= BrightnessProperty;
    if (capabilities & MC_CAPS_CONTRAST)
        result |= ContrastProperty;
    return result;
}

float WinMonitor::property(Property property) const {
    WinDdcTriplet triplet;
    if (!m_monitor->read(property, &triplet))
        return NAN;

    if (triplet.min >= triplet.max)
        return NAN; /* Just a sanity check. */

    m_cachedProperties[property] = triplet;
    return mapFromNative(triplet);
}

void WinMonitor::setProperty(Property property, float value) {
    if (!m_cachedProperties[property]) {
        this->property(property);
        if (!m_cachedProperties[property])
            return; /* Likely unsupported. */
    }

    m_monitor->write(property, mapToNative(*m_cachedProperties[property], value));
}
