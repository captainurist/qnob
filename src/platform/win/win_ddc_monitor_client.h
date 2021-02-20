#pragma once

#include <memory>

#include <QtCore/QObject>

#include <platform/platform_monitor.h>

#include "win_ddc_triplet.h"

class WinDdcMonitorQueue;

class WinDdcMonitorClient: public QObject {
    Q_OBJECT
public:
    WinDdcMonitorClient(std::shared_ptr<WinDdcMonitorQueue> queue);

    void read(PlatformMonitor::Property property);
    void write(PlatformMonitor::Property property, DWORD value);

signals:
    void queueChanged();

private:
    std::shared_ptr<WinDdcMonitorQueue> m_queue;
};
