#include "volume_setting_backend.h"

#include <cassert>

#include <platform/platform.h>

VolumeSettingBackend::VolumeSettingBackend(QObject* parent) :
    SettingBackend(parent)
{
    connect(platform()->volumeControl(), &PlatformVolumeControl::changedExternally, this, &SettingBackend::changedExternally);
}

VolumeSettingBackend::~VolumeSettingBackend() {}

double VolumeSettingBackend::value() const {
    return platform()->volumeControl()->volume();
}

void VolumeSettingBackend::setValue(double value) {
    assert(value >= 0.0 && value <= 1.0);

    platform()->volumeControl()->setVolume(value);
}

bool VolumeSettingBackend::isEnabled() const {
    return !platform()->volumeControl()->isMuted() && !qFuzzyIsNull(platform()->volumeControl()->volume());
}

void VolumeSettingBackend::setEnabled(bool value) {
    platform()->volumeControl()->setMuted(!value);
}

bool VolumeSettingBackend::isInitialized() const {
    return true;
}

