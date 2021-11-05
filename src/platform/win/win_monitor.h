#pragma once

#include <wtypes.h>
#include <physicalmonitorenumerationapi.h> /* For PHYSICAL_MONITOR. */

#include <optional>
#include <array>

#include <platform/platform_monitor.h>

#include "win_ddc_triplet.h"

class WinDdcMonitor;

class WinMonitor : public PlatformMonitor {
    Q_OBJECT
public:
    WinMonitor(const QString& deviceId, PHYSICAL_MONITOR physicalMonitor, QObject* parent);
    virtual ~WinMonitor();

    virtual QString deviceId() const override;
    virtual QString name() const override;
    virtual PlatformMonitorProperties capabilities() const override;
    virtual float property(PlatformMonitorProperty property) const override;
    virtual void setProperty(PlatformMonitorProperty property, float value) override;

private:
    void capabilitiesInternal(DWORD* capabilities) const;
    bool propertyInternal(PlatformMonitorProperty property, WinDdcTriplet* value) const;
    bool setPropertyInternal(PlatformMonitorProperty property, DWORD value);

    template<class T>
    bool maybeRetryCall(T&& call) const;

private:
    QString m_deviceId;
    PHYSICAL_MONITOR m_physicalMonitor;
    mutable size_t m_retriesLeft = 0;
    mutable std::array<std::optional<WinDdcTriplet>, MonitorPropertyCount> m_cachedProperties;
};
