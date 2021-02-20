#pragma once

#include <cmath> /* For NAN. */
#include <memory>

#include <platform/platform_monitor.h>

#include "win_ddc_triplet.h"

class WinDdcMonitor;
class WinDdcMonitorClient;
class WinDdcMonitorServer;

class WinMonitor: public PlatformMonitor {
    Q_OBJECT
public:
    WinMonitor(std::shared_ptr<QThread> sharedThread, std::unique_ptr<WinDdcMonitor> monitor);
    virtual ~WinMonitor();

    virtual QString name() const override;
    virtual bool supported(Property property) const override;
    virtual float property(Property property) const override;
    virtual void setProperty(Property property, float value) override;

private:
    void updateProperty(Property property);
    void handleReadCompleted(bool ok, PlatformMonitor::Property property, WinDdcTriplet value);
    void handleWriteCompleted(bool ok, PlatformMonitor::Property property, DWORD value);

    static DWORD mapToNative(const WinDdcTriplet& triplet, float value);
    static float mapFromNative(const WinDdcTriplet& triplet);

    struct PropertyState {
        WinDdcTriplet triplet;
        float pendingWrite = NAN;
    };

private:
    std::shared_ptr<QThread> m_sharedThread;
    std::unique_ptr<WinDdcMonitorClient> m_client;
    WinDdcMonitorServer* m_server;
    DWORD m_capabilities;
    QString m_name;
    std::array<PropertyState, PropertyCount> m_properties;
};
