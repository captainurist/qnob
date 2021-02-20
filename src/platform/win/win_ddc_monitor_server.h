#pragma once

#include <memory>

#include <QtCore/QObject>

#include <platform/platform_monitor.h>

#include "win_ddc_triplet.h"

class WinDdcMonitor;
class WinDdcMonitorQueue;

class WinDdcMonitorServer : public QObject {
    Q_OBJECT
public:
    WinDdcMonitorServer(std::unique_ptr<WinDdcMonitor> monitor, std::shared_ptr<WinDdcMonitorQueue> queue);
    virtual ~WinDdcMonitorServer();

    Q_SLOT void processQueue();

signals:
    void readCompleted(bool ok, PlatformMonitor::Property property, WinDdcTriplet value);
    void writeCompleted(bool ok, PlatformMonitor::Property property, DWORD value);

private:
    std::unique_ptr<WinDdcMonitor> m_monitor;
    std::shared_ptr<WinDdcMonitorQueue> m_queue;
};
