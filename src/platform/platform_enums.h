#pragma once

#include <util/bitset_flags.h>

enum PlatformMonitorProperty {
    BrightnessMonitorProperty,
    ContrastMonitorProperty,
    MonitorPropertyCount
};
X_DECLARE_BITSET_FLAGS(PlatformMonitorProperties, PlatformMonitorProperty, MonitorPropertyCount);
X_DELARE_OPERATORS_FOR_BITSET_FLAGS(PlatformMonitorProperties)

enum PlatformStandardControl {
    AudioTrayIconControl,
    NativeOsdControl,
    NonStandardControl
};

