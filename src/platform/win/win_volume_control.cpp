#include "win_volume_control.h"

#include <combaseapi.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#include <algorithm>

#include "com_ptr.h"

static const GUID GUID_QnobAudioEvent = {0x3bd43a3e, 0xd585, 0x4354, {0xcb, 0xf3, 0x55, 0xba, 0x11, 0xfc, 0x84, 0x12}};
static const CLSID CLSID_MMDeviceEnumerator = {0xBCDE0395, 0xE52F, 0x467C, {0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E}};


WinVolumeControl::WinVolumeControl() {
    if (!SUCCEEDED(CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_enumerator))))
        return;

    if (!SUCCEEDED(m_enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, m_defaultDevice.mutablePtr())))
        return;

    if (!SUCCEEDED(m_defaultDevice->Activate(m_volumeControl.staticId(), CLSCTX_INPROC_SERVER, nullptr, m_volumeControl.mutableVoidPtr())))
        return;

}

WinVolumeControl::~WinVolumeControl() {
}

float WinVolumeControl::volume() const {
    if (!m_volumeControl)
        return 0;

    float result;
    if (!SUCCEEDED(m_volumeControl->GetMasterVolumeLevelScalar(&result)))
        return 0;

    return result;
}

void WinVolumeControl::setVolume(float volume) {
    if (!m_volumeControl)
        return;

    m_volumeControl->SetMasterVolumeLevelScalar(std::clamp(volume, 0.0f, 1.0f), &GUID_QnobAudioEvent);
}

bool WinVolumeControl::isMuted() const {
    if (!m_volumeControl)
        return true;

    BOOL muted = FALSE;
    if(!SUCCEEDED(m_volumeControl->GetMute(&muted)))
        return false;

    return muted;
}

void WinVolumeControl::setMuted(bool muted) {
    if (!m_volumeControl)
        return;

    m_volumeControl->SetMute(muted, &GUID_QnobAudioEvent);
}

