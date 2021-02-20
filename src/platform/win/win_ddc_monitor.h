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

// TODO: WM_DISPLAYCHANGE is sent when # of monitors is changed.

// TODO

/*physical monitor -- for actual api calls.
shared_ptr'd

ddc queue -- per-monitor queue, basically just does uniq for calls

ddc queue processor - qobject, contains shared_ptr to monitor, implements thread-safe api with callbacks must be moved to the processing thread.

win_monitor_manager
contains qthread
contains queue processors
listens to change events (destroys monitors & queues in this case)
exposes sane api
*/


