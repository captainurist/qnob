#pragma once

#include <memory>

#include <QtCore/QObject>

#include <platform/platform_monitor.h>

class MonitorQueue;

class MonitorServer : public QObject {
    Q_OBJECT
public:
    MonitorServer(std::unique_ptr<PlatformMonitor> monitor, std::shared_ptr<MonitorQueue> queue);
    virtual ~MonitorServer();

    Q_SLOT void processQueue();

signals:
    void capabilitiesCompleted(PlatformMonitorProperties capabilities);
    void readCompleted(PlatformMonitorProperty property, float value);
    void writeCompleted(PlatformMonitorProperty property, float value);

private:
    std::unique_ptr<PlatformMonitor> m_monitor;
    std::shared_ptr<MonitorQueue> m_queue;
};

