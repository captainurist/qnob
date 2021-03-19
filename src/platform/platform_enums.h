#pragma once

#include <util/bitset_flags.h>

enum PlatformMonitorProperty {
    BrightnessMonitorProperty,
    ContrastMonitorProperty,
    MonitorPropertyCount
};
QB_DECLARE_BITSET_FLAGS(PlatformMonitorProperties, PlatformMonitorProperty, MonitorPropertyCount);
QB_DELARE_OPERATORS_FOR_BITSET_FLAGS(PlatformMonitorProperties)

enum PlatformStandardTrayIcon {
    AudioTrayIcon
};
