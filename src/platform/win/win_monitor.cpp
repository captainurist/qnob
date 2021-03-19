#include "win_monitor.h"

#include <cmath> /* For NAN. */
#include <algorithm> /* For std::clamp. */

#include <highlevelmonitorconfigurationapi.h>

#include "win_error.h"

static DWORD mapToNative(const WinDdcTriplet& triplet, float value) {
    assert(triplet.min < triplet.max);

    return triplet.min + (triplet.max - triplet.min) * std::clamp(value, 0.0f, 1.0f);
}

static float mapFromNative(const WinDdcTriplet& triplet) {
    assert(triplet.min < triplet.max);

    return (triplet.current - triplet.min) * 1.0f / (triplet.max - triplet.min);
}

WinMonitor::WinMonitor(const QString& deviceId, PHYSICAL_MONITOR physicalMonitor):
    m_deviceId(deviceId),
    m_physicalMonitor(physicalMonitor)
{}

WinMonitor::~WinMonitor() {
    apicall(DestroyPhysicalMonitor(m_physicalMonitor.hPhysicalMonitor));
}

QString WinMonitor::deviceId() const {
    return m_deviceId;
}

QString WinMonitor::name() const {
    return QString::fromWCharArray(m_physicalMonitor.szPhysicalMonitorDescription);
}

PlatformMonitorProperties WinMonitor::capabilities() const {
    DWORD capabilities;
    capabilitiesInternal(&capabilities);

    PlatformMonitorProperties result;
    if (capabilities & MC_CAPS_BRIGHTNESS)
        result |= BrightnessMonitorProperty;
    if (capabilities & MC_CAPS_CONTRAST)
        result |= ContrastMonitorProperty;
    return result;
}

float WinMonitor::property(PlatformMonitorProperty property) const {
    WinDdcTriplet triplet;
    if (!propertyInternal(property, &triplet))
        return NAN;

    if (triplet.min >= triplet.max)
        return NAN; /* Just a sanity check. */

    m_cachedProperties[property] = triplet;
    return mapFromNative(triplet);
}

void WinMonitor::setProperty(PlatformMonitorProperty property, float value) {
    if (!m_cachedProperties[property]) {
        this->property(property);
        if (!m_cachedProperties[property])
            return; /* Likely unsupported. */
    }

    setPropertyInternal(property, mapToNative(*m_cachedProperties[property], value));
}

void WinMonitor::capabilitiesInternal(DWORD* capabilities) const {
    DWORD colorTemperatures; /* Discarded for now. */

    if (!apicall(GetMonitorCapabilities(m_physicalMonitor.hPhysicalMonitor, capabilities, &colorTemperatures))) {
        /* This means that the monitor doesn't support DDC/CI. */
        *capabilities = 0;
    }
}

bool WinMonitor::propertyInternal(PlatformMonitorProperty property, WinDdcTriplet* value) const {
    switch (property) {
    case BrightnessMonitorProperty:
        return apicall(GetMonitorBrightness(m_physicalMonitor.hPhysicalMonitor, &value->min, &value->current, &value->max));
    case ContrastMonitorProperty:
        return apicall(GetMonitorContrast(m_physicalMonitor.hPhysicalMonitor, &value->min, &value->current, &value->max));
    default:
        assert(false);
        return false;
    }
}

bool WinMonitor::setPropertyInternal(PlatformMonitorProperty property, DWORD value) {
    switch (property) {
    case BrightnessMonitorProperty:
        return apicall(SetMonitorBrightness(m_physicalMonitor.hPhysicalMonitor, value));
    case ContrastMonitorProperty:
        return apicall(SetMonitorContrast(m_physicalMonitor.hPhysicalMonitor, value));
    default:
        assert(false);
        return false;
    }
}
