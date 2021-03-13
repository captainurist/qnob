#pragma once

#include "shaft.h"

class VolumeShaft : public Shaft {
    Q_OBJECT
public:
    VolumeShaft();
    virtual ~VolumeShaft();

    virtual double value() const override;
    virtual void setValue(double value) override;

    virtual bool isEnabled() const override;
    virtual void setEnabled(bool value) override;
};
