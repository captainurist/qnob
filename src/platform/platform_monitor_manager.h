#pragma once

#include <QtCore/QObject>

#include "platform_monitor.h"

class PlatformMonitorManager : public QObject {
    Q_OBJECT
public:
    virtual std::vector<std::unique_ptr<PlatformMonitor>> enumerateMonitors() = 0;

signals:
    void monitorsChanged();
};
