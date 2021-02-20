#pragma once

#include "shaft.h"

class PlatformVolumeControl;

class VolumeShaft : public Shaft {
    Q_OBJECT
public:
    VolumeShaft();
    virtual ~VolumeShaft();

    virtual double value() const override;
    virtual void setValue(double value) override;

    virtual bool enabled() const override;
    virtual void setEnabled(bool value) override;

private:
    std::unique_ptr<PlatformVolumeControl> m_volumeControl;
};
