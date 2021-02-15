#pragma once

#include "shaft.h"

class PlatformBrightnessControl;

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
    PlatformBrightnessControl* m_brightnessControl;
};
