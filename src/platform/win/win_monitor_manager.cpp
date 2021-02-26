#include "win_monitor_manager.h"

#include <physicalmonitorenumerationapi.h> /* For EnumDisplayMonitors. */

#include <QtCore/QVarLengthArray>

#include "win_monitor.h"
#include "win_error.h"

static BOOL CALLBACK EnumDisplayMonitorsProc(HMONITOR handle, HDC /*hdc*/, LPRECT /*rect*/, LPARAM data) {
    reinterpret_cast<std::vector<HMONITOR>*>(data)->push_back(handle);
    return TRUE;
}

static std::vector<HMONITOR> enumDisplayMonitors() {
    std::vector<HMONITOR> result;
    if (!succeeded(EnumDisplayMonitors(nullptr, nullptr, EnumDisplayMonitorsProc, reinterpret_cast<LPARAM>(&result))))
        result.clear();
    return result;
}

WinMonitorManager::WinMonitorManager() {}

std::vector<std::unique_ptr<PlatformMonitor>> WinMonitorManager::enumerateMonitors() {
    std::vector<std::unique_ptr<PlatformMonitor>> result;

    /* We can also get hardware IDs by matching monitors here with what EnumDisplayDevices returns,
     * but there is little point. */

    QVarLengthArray<PHYSICAL_MONITOR, 8> physicalMonitors;
    for (HMONITOR displayMonitor : enumDisplayMonitors()) {
        DWORD count = 0;

        /* We can get display index here from MONITORINFOEXW::szDevice after a call to GetMonitorInfoW.
         * But again, there is little point for now. */

        if (!succeeded(GetNumberOfPhysicalMonitorsFromHMONITOR(displayMonitor, &count)) || count == 0)
            continue;

        physicalMonitors.resize(count);

        if (!succeeded(GetPhysicalMonitorsFromHMONITOR(displayMonitor, count, physicalMonitors.data())))
            continue;

        for (PHYSICAL_MONITOR& physicalMonitor : physicalMonitors)
            result.emplace_back(new WinMonitor(physicalMonitor));
    }

    return result;
}
