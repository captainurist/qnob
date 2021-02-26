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
    WinMonitor(PHYSICAL_MONITOR physicalMonitor);
    virtual ~WinMonitor();

    virtual QString name() const override;
    virtual Properties capabilities() const override;
    virtual float property(Property property) const override;
    virtual void setProperty(Property property, float value) override;

private:
    void capabilitiesInternal(DWORD* capabilities) const;
    bool propertyInternal(Property property, WinDdcTriplet * value) const;
    bool setPropertyInternal(Property property, DWORD value);

private:
    PHYSICAL_MONITOR m_physicalMonitor;
    mutable std::array<std::optional<WinDdcTriplet>, PlatformMonitor::PropertyCount> m_cachedProperties;
};
