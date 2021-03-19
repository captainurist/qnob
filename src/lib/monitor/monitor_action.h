#pragma once

#include <platform/platform_enums.h>

struct MonitorAction {
    enum Type {
        ReadProperty,
        WriteProperty,
        ReadCapabilities
    };

    Type type = ReadProperty;
    PlatformMonitorProperty property = BrightnessMonitorProperty;
    float value = 0.0f;
};
