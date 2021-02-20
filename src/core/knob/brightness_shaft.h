#pragma once

#include "shaft.h"

class PlatformMonitorManager;
class PlatformMonitor;

class BrightnessShaft : public Shaft {
    Q_OBJECT
public:
    BrightnessShaft();
    virtual ~BrightnessShaft();

    virtual double value() const override;
    virtual void setValue(double value) override;

    virtual bool enabled() const override;
    virtual void setEnabled(bool value) override;

private:
    std::unique_ptr<PlatformMonitorManager> m_monitorManager;
    std::vector<std::unique_ptr<PlatformMonitor>> m_monitors;
    double m_value;
};
