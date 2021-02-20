#pragma once

#include <wtypes.h>

#include <platform/platform_monitor.h>

struct WinDdcMonitorAction {
    enum Type {
        Read,
        Write
    };

    PlatformMonitor::Property property;
    Type type;
    DWORD value;
};
