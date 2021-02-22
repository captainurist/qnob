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

    /**
     * \returns                         Name of the monitor, as shown in device manager.
     */
    QString name() const;

    /**
     * Reads monitor capabilities. Might take over a second to complete.
     *
     * \param[out] capabilities         Monitor capabilities.
     */
    void readCapabilities(DWORD* capabilities);

    /**
     * Reads a DDC/CI property. Might take up to 50ms to complete.
     *
     * \param property                  Property to read.
     * \param[out]                      Property value.
     * \returns                         Whether the operation was successful.
     */
    bool read(PlatformMonitor::Property property, WinDdcTriplet* value);

    /**
     * Writes a DDC/CI property. Might take up to 50ms to complete.
     *
     * \param property                  Property to write.
     * \param value                     New value for the property.
     * \returns                         Whether the operation was successful.
     */
    bool write(PlatformMonitor::Property property, DWORD value);

private:
    PHYSICAL_MONITOR m_physicalMonitor;
};
