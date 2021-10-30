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

    virtual QString deviceId() const override;
    virtual QString name() const override;
    virtual PlatformMonitorProperties capabilities() const override;
    virtual float property(PlatformMonitorProperty property) const override;
    virtual void setProperty(PlatformMonitorProperty property, float value) override;

signals:
    void initialized();

private:
    Q_SIGNAL void notifyServer();

    void handleInitializationReply();
    void handleCapabilitiesCompleted(PlatformMonitorProperties capabilities);
    void handleReadCompleted(PlatformMonitorProperty property, float value);
    void handleWriteCompleted(PlatformMonitorProperty property, float value);

private:
    std::unique_ptr<QThread> m_thread;
    std::shared_ptr<MonitorQueue> m_queue;
    MonitorServer* m_server = nullptr;
    int m_pendingInitializationRequests = 0;
    QString m_cachedDeviceId;
    QString m_cachedName;
    PlatformMonitorProperties m_cachedCapabilities;
    std::array<float, MonitorPropertyCount> m_cachedProperties;
};
