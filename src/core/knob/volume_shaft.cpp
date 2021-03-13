#include "volume_shaft.h"

#include <cassert>

#include <platform/platform.h>

VolumeShaft::VolumeShaft() {
    connect(platform()->volumeControl(), &PlatformVolumeControl::changedExternally, this, &Shaft::changedExternally);
}

VolumeShaft::~VolumeShaft()
{}

double VolumeShaft::value() const {
    return platform()->volumeControl()->volume();
}

void VolumeShaft::setValue(double value) {
    assert(value >= 0.0 && value <= 1.0);

    platform()->volumeControl()->setVolume(value);
}

bool VolumeShaft::isEnabled() const {
    return !platform()->volumeControl()->isMuted() && !qFuzzyIsNull(platform()->volumeControl()->volume());
}

void VolumeShaft::setEnabled(bool value) {
    platform()->volumeControl()->setMuted(!value);
}
