#include "volume_shaft.h"

#include <cassert>

#include <platform/platform.h>

VolumeShaft::VolumeShaft():
    m_volumeControl(platform()->createVolumeControl())
{
    connect(m_volumeControl.get(), &PlatformVolumeControl::notificationReceived, this, &Shaft::notificationReceived);
}

VolumeShaft::~VolumeShaft()
{}

double VolumeShaft::value() const {
    return m_volumeControl->volume();
}

void VolumeShaft::setValue(double value) {
    assert(value >= 0.0 && value <= 1.0);

    m_volumeControl->setVolume(value);
}

bool VolumeShaft::enabled() const {
    return !m_volumeControl->isMuted() && !qFuzzyIsNull(m_volumeControl->volume());
}

void VolumeShaft::setEnabled(bool value) {
    m_volumeControl->setMuted(!value);
}
