#pragma once

#include <physicalmonitorenumerationapi.h>

#include <memory>
#include <vector>

// TODO: WM_DISPLAYCHANGE is sent when # of monitors is changed.

/**
 * A thin wrapper around high-level monitor configuration API.
 *
 * Note that all functions in this class log on error, so the callee doesn't need to do this.
 */
class WinPhysicalMonitor {
public:
    WinPhysicalMonitor(const PHYSICAL_MONITOR& physicalMonitor);
    ~WinPhysicalMonitor();

    static std::vector<std::unique_ptr<WinPhysicalMonitor>> enumerateMonitors();

    void readCapabilities(DWORD* capabilities, DWORD* colorTemperatures);

    /* Note: functions below take up to 50ms to complete as noted in the corresponding API docs. */

    bool readBrightness(DWORD* minimum, DWORD* current, DWORD* maximum);
    bool setBrightness(DWORD brightness);

    bool readContrast(DWORD* minimum, DWORD* current, DWORD* maximum);
    bool setContrast(DWORD contrast);

private:
    PHYSICAL_MONITOR m_physicalMonitor;
};
