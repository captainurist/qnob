#pragma once

#include <platform/platform_monitor.h>

struct MonitorAction {
    enum Type {
        ReadProperty,
        WriteProperty,
        ReadCapabilities
    };

    Type type = ReadProperty;
    PlatformMonitor::Property property = PlatformMonitor::BrightnessProperty;
    float value = 0.0f;
};
