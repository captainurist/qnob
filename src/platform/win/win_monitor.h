#pragma once

#include <wtypes.h>

#include <optional>
#include <array>

#include <platform/platform_monitor.h>

#include "win_ddc_triplet.h"

class WinDdcMonitor;

class WinMonitor : public PlatformMonitor {
    Q_OBJECT
public:
    WinMonitor(std::unique_ptr<WinDdcMonitor> monitor);
    virtual ~WinMonitor();

    virtual QString name() const override;
    virtual Properties capabilities() const override;
    virtual float property(Property property) const override;
    virtual void setProperty(Property property, float value) override;

private:
    std::unique_ptr<WinDdcMonitor> m_monitor;
    mutable std::array<std::optional<WinDdcTriplet>, PlatformMonitor::PropertyCount> m_cachedProperties;
};
