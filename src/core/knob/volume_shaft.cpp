#include "volume_shaft.h"

#include <platform/platform.h>

VolumeShaft::VolumeShaft() {
    m_volumeControl = platform()->volumeControl();

    connect(m_volumeControl, &PlatformVolumeControl::notificationReceived, this, &Shaft::notificationReceived);
}

VolumeShaft::~VolumeShaft()
{}

double VolumeShaft::value() const {
    return m_volumeControl->volume();
}

void VolumeShaft::setValue(double value) {
    m_volumeControl->setVolume(value);
}

bool VolumeShaft::enabled() const {
    return !m_volumeControl->isMuted();
}

void VolumeShaft::setEnabled(bool value) {
    m_volumeControl->setMuted(!value);
}

