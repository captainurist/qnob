#pragma once

#include <cmath> /* For NAN. */
#include <memory>

#include <platform/platform_monitor.h>

class MonitorQueue;
class MonitorServer;

class Monitor: public PlatformMonitor {
    Q_OBJECT
public:
    Monitor(std::unique_ptr<PlatformMonitor> monitor);
    virtual ~Monitor();

    bool isInitialized() const;

    virtual QString name() const override;
    virtual Properties capabilities() const override;
    virtual float property(Property property) const override;
    virtual void setProperty(Property property, float value) override;

signals:
    void initialized();

private:
    Q_SIGNAL void notifyServer();

    void handleInitializationReply();
    void handleCapabilitiesCompleted(PlatformMonitor::Properties capabilities);
    void handleReadCompleted(PlatformMonitor::Property property, float value);
    void handleWriteCompleted(PlatformMonitor::Property property, float value);

private:
    std::unique_ptr<QThread> m_thread;
    std::shared_ptr<MonitorQueue> m_queue;
    int m_pendingInitializationRequests = 0;
    QString m_cachedName;
    Properties m_cachedCapabilities;
    std::array<float, PropertyCount> m_cachedProperties;
};
