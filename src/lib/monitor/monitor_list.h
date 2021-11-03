#pragma once

#include <vector>
#include <memory>

#include <platform/platform_monitor.h>


using MonitorList = std::vector<std::unique_ptr<PlatformMonitor>>;

