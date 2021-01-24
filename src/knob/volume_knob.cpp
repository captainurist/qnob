#include "volume_knob.h"

#include <platform/platform.h>

VolumeKnob::VolumeKnob(QObject* parent) : 
    Knob(parent)
{
    m_volumeControl = qPlatform()->volumeControl();
}

VolumeKnob::~VolumeKnob() 
{}

QByteArray VolumeKnob::id() const {
    return "volume";
}

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

