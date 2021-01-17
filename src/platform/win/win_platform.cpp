#include "win_platform.h"

#include <QtCore/QtGlobal>

#include "com.h"
#include "win_volume_control.h"

WinPlatform::WinPlatform() {
    m_com.reset(new Com());
    m_volumeControl.reset(new WinVolumeControl());
}

WinPlatform::~WinPlatform() {
}

VolumeControl* WinPlatform::volumeControl() const {
    return m_volumeControl.get();
}

Platform* Platform::instance() {
    static WinPlatform platform;
    return &platform;
}
