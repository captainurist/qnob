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
    void capabilitiesCompleted(PlatformMonitor::Properties capabilities);
    void readCompleted(PlatformMonitor::Property property, float value);
    void writeCompleted(PlatformMonitor::Property property, float value);

private:
    std::unique_ptr<PlatformMonitor> m_monitor;
    std::shared_ptr<MonitorQueue> m_queue;
};

Q_DECLARE_METATYPE(PlatformMonitor::Properties);
Q_DECLARE_METATYPE(PlatformMonitor::Property);
