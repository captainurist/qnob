#pragma once

#include "knob.h"

#include <QtCore/QScopedPointer>

class PlatformVolumeControl;

class VolumeKnob : public Knob {
    Q_OBJECT
public:
    VolumeKnob();
    virtual ~VolumeKnob();

    virtual double value() const override;
    virtual void setValue(double value) override;

    virtual bool enabled() const override;
    virtual void setEnabled(bool value) override;

private:
    PlatformVolumeControl* m_volumeControl;
};
