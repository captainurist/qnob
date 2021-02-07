#include "volume_knob.h"

#include <platform/platform.h>

VolumeKnob::VolumeKnob() :
    Knob(lit("volume"))
{
    m_volumeControl = qPlatform()->volumeControl();
}

VolumeKnob::~VolumeKnob()
{}

double VolumeKnob::value() const {
    return m_volumeControl->volume();
}

void VolumeKnob::setValue(double value) {
    m_volumeControl->setVolume(value);
}

bool VolumeKnob::enabled() const {
    return !m_volumeControl->isMuted();
}

void VolumeKnob::setEnabled(bool value) {
    m_volumeControl->setMuted(!value);
}

