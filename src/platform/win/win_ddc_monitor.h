#pragma once

#include <physicalmonitorenumerationapi.h>

#include <vector>
#include <memory>

#include <platform/platform_monitor.h>

#include "win_ddc_triplet.h"

/**
 * A thin wrapper around high-level monitor configuration API.
 *
 * All functions in this class log on error, so the callee doesn't need to do this.
 */
class WinDdcMonitor {
public:
    /**
     * \param physicalMonitor           Physical monitor to construct this object for.
     *                                  It will be freed in destructor.
     */
    WinDdcMonitor(const PHYSICAL_MONITOR& physicalMonitor);
    ~WinDdcMonitor();

    static std::vector<std::unique_ptr<WinDdcMonitor>> enumerateMonitors();

    QString name() const;

    void readCapabilities(DWORD* capabilities, DWORD* colorTemperatures);

    /* Functions below take up to 50ms to complete as noted in the corresponding API docs. */

    bool read(PlatformMonitor::Property property, WinDdcTriplet* value);
    bool write(PlatformMonitor::Property property, DWORD value);

private:
    PHYSICAL_MONITOR m_physicalMonitor;
};
