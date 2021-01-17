#include "win_volume_control.h"

#include <combaseapi.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#include "com_ptr.h"

static const GUID GUID_QnobAudioEvent = {0x3bd43a3e, 0xd585, 0x4354, {0xcb, 0xf3, 0x55, 0xba, 0x11, 0xfc, 0x84, 0x12}};
static const CLSID CLSID_MMDeviceEnumerator = {0xBCDE0395, 0xE52F, 0x467C, {0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E}};


WinVolumeControl::WinVolumeControl() {
    if (!SUCCEEDED(CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&enumerator))))
        return;

    if (!SUCCEEDED(enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, defaultDevice.mutablePtr())))
        return;

    if (!SUCCEEDED(defaultDevice->Activate(volumeControl.staticId(), CLSCTX_INPROC_SERVER, nullptr, volumeControl.mutableVoidPtr())))
        return;

}

WinVolumeControl::~WinVolumeControl() {
}

float WinVolumeControl::volume() const {
    if (!volumeControl)
        return 0;

    float result;
    if (!SUCCEEDED(volumeControl->GetMasterVolumeLevelScalar(&result)))
        return 0;

    return result;
}

void WinVolumeControl::setVolume(float volume) {
    if (!volumeControl)
        return;

    volumeControl->SetMasterVolumeLevelScalar(qBound(0.0f, volume, 1.0f), &GUID_QnobAudioEvent);
}

bool WinVolumeControl::isMuted() const {
    if (!volumeControl)
        return true;

    BOOL muted = FALSE;
    if(!SUCCEEDED(volumeControl->GetMute(&muted)))
        return false;

    return muted;
}

void WinVolumeControl::setMuted(bool muted) {
    if (!volumeControl)
        return;

    volumeControl->SetMute(muted, &GUID_QnobAudioEvent);
}

