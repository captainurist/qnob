#pragma once

#include <physicalmonitorenumerationapi.h>

#include <memory>
#include <vector>

class WinPhysicalMonitorPrivate;

class WinPhysicalMonitor {
public:
    WinPhysicalMonitor() = default;
    WinPhysicalMonitor(const WinPhysicalMonitor& other) = default;

    static std::vector<WinPhysicalMonitor> enumerateMonitors();

    bool supportsBrightness() const;

    // TODO: what about the functions that take 40ms? they break all my APIs T_T

protected:
    WinPhysicalMonitor(PHYSICAL_MONITOR* monitor, std::shared_ptr<WinPhysicalMonitorPrivate> d);

private:
    // TODO: wrong, should store PHYSICAL_MONITOR right in d!

    PHYSICAL_MONITOR* m_monitor = nullptr;
    std::shared_ptr<WinPhysicalMonitorPrivate> m_d;
};
