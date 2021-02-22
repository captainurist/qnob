#include "win_ddc_monitor.h"

#include <highlevelmonitorconfigurationapi.h>

#include <QtCore/QVarLengthArray>

#include <util/debug.h>

#include "win_error.h"

namespace {

BOOL CALLBACK EnumDisplayMonitorsProc(HMONITOR handle, HDC /*hdc*/, LPRECT /*rect*/, LPARAM data) {
    reinterpret_cast<std::vector<HMONITOR>*>(data)->push_back(handle);
    return TRUE;
}

std::vector<HMONITOR> enumDisplayMonitors() {
    std::vector<HMONITOR> result;
    if (!succeeded(EnumDisplayMonitors(nullptr, nullptr, EnumDisplayMonitorsProc, reinterpret_cast<LPARAM>(&result))))
        result.clear();
    return result;
}

}

WinDdcMonitor::WinDdcMonitor(const PHYSICAL_MONITOR& physicalMonitor):
    m_physicalMonitor(physicalMonitor)
{}

WinDdcMonitor::~WinDdcMonitor() {
    succeeded(DestroyPhysicalMonitor(m_physicalMonitor.hPhysicalMonitor));
}

std::vector<std::unique_ptr<WinDdcMonitor>> WinDdcMonitor::enumerateMonitors() {
    std::vector<std::unique_ptr<WinDdcMonitor>> result;

    QVarLengthArray<PHYSICAL_MONITOR, 8> physicalMonitors;
    for (HMONITOR displayMonitor : enumDisplayMonitors()) {
        DWORD count = 0;
        if (!succeeded(GetNumberOfPhysicalMonitorsFromHMONITOR(displayMonitor, &count)) || count == 0)
            continue;

        physicalMonitors.resize(count);

        if (!succeeded(GetPhysicalMonitorsFromHMONITOR(displayMonitor, count, physicalMonitors.data())))
            continue;

        for (PHYSICAL_MONITOR& physicalMonitor : physicalMonitors)
            result.emplace_back(new WinDdcMonitor(physicalMonitor));
    }

    return result;
}

QString WinDdcMonitor::name() const {
    return QString::fromWCharArray(m_physicalMonitor.szPhysicalMonitorDescription);
}

void WinDdcMonitor::readCapabilities(DWORD* capabilities) {
    DWORD colorTemperatures; /* Discarded for now. */

    if (!succeeded(GetMonitorCapabilities(m_physicalMonitor.hPhysicalMonitor, capabilities, &colorTemperatures))) {
        /* This means that the monitor doesn't support DDC/CI. */
        *capabilities = 0;
    }
}

bool WinDdcMonitor::read(PlatformMonitor::Property property, WinDdcTriplet* value) {
    switch (property) {
    case PlatformMonitor::BrightnessProperty:
        return succeeded(GetMonitorBrightness(m_physicalMonitor.hPhysicalMonitor, &value->min, &value->current, &value->max));
    case PlatformMonitor::ContrastProperty:
        return succeeded(GetMonitorContrast(m_physicalMonitor.hPhysicalMonitor, &value->min, &value->current, &value->max));
    default:
        QB_VERIFY(false);
    }
}

bool WinDdcMonitor::write(PlatformMonitor::Property property, DWORD value) {
    switch (property) {
    case PlatformMonitor::BrightnessProperty:
        return succeeded(SetMonitorBrightness(m_physicalMonitor.hPhysicalMonitor, value));
    case PlatformMonitor::ContrastProperty:
        return succeeded(SetMonitorContrast(m_physicalMonitor.hPhysicalMonitor, value));
    default:
        QB_VERIFY(false);
    }
}
