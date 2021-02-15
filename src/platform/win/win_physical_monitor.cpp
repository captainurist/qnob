#include "win_physical_monitor.h"

#include <highlevelmonitorconfigurationapi.h>

#include <QtCore/QVarLengthArray>

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

WinPhysicalMonitor::WinPhysicalMonitor(const PHYSICAL_MONITOR& physicalMonitor):
    m_physicalMonitor(physicalMonitor)
{}

WinPhysicalMonitor::~WinPhysicalMonitor() {
    succeeded(DestroyPhysicalMonitor(m_physicalMonitor.hPhysicalMonitor));
}

std::vector<std::unique_ptr<WinPhysicalMonitor>> WinPhysicalMonitor::enumerateMonitors() {
    std::vector<std::unique_ptr<WinPhysicalMonitor>> result;

    QVarLengthArray<PHYSICAL_MONITOR, 8> physicalMonitors;
    for (HMONITOR displayMonitor : enumDisplayMonitors()) {
        DWORD count = 0;
        if (!succeeded(GetNumberOfPhysicalMonitorsFromHMONITOR(displayMonitor, &count)) || count == 0)
            continue;

        physicalMonitors.resize(count);

        if (!succeeded(GetPhysicalMonitorsFromHMONITOR(displayMonitor, count, physicalMonitors.data())))
            continue;

        for (PHYSICAL_MONITOR& physicalMonitor : physicalMonitors)
            result.emplace_back(new WinPhysicalMonitor(physicalMonitor));
    }

    return result;
}

void WinPhysicalMonitor::readCapabilities(DWORD* capabilities, DWORD* colorTemperatures) {
    if (!succeeded(GetMonitorCapabilities(m_physicalMonitor.hPhysicalMonitor, capabilities, colorTemperatures))) {
        /* This means that the monitor doesn't support DDC/CI. */
        *capabilities = 0;
        *colorTemperatures = 0;
    }
}

bool WinPhysicalMonitor::readBrightness(DWORD* minimum, DWORD* current, DWORD* maximum) {
    return succeeded(GetMonitorBrightness(m_physicalMonitor.hPhysicalMonitor, minimum, current, maximum));
}

bool WinPhysicalMonitor::setBrightness(DWORD brightness) {
    return succeeded(SetMonitorBrightness(m_physicalMonitor.hPhysicalMonitor, brightness));
}

bool WinPhysicalMonitor::readContrast(DWORD* minimum, DWORD* current, DWORD* maximum) {
    return succeeded(GetMonitorContrast(m_physicalMonitor.hPhysicalMonitor, minimum, current, maximum));
}

bool WinPhysicalMonitor::setContrast(DWORD contrast) {
    return succeeded(SetMonitorContrast(m_physicalMonitor.hPhysicalMonitor, contrast));
}
