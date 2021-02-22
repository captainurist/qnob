#pragma once

#include <QtCore/QFuture>

class PlatformMonitor;
class PlatformMonitorManager;

class MonitorManager {
public:
    using MonitorList = std::vector<std::unique_ptr<PlatformMonitor>>;

    static QFuture<MonitorList> enumerateMonitors(PlatformMonitorManager* manager);
};
