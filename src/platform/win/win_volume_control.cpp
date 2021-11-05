#include "win_volume_control.h"

#include <combaseapi.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#include <cmath>
#include <algorithm> /* For std::clamp. */

#include "win_volume_event_handler.h"
#include "win_error.h"

static constexpr GUID GUID_QnobAudioEvent = {0x3bd43a3e, 0xd585, 0x4354, {0xcb, 0xf3, 0x55, 0xba, 0x11, 0xfc, 0x84, 0x12}};

WinVolumeControl::WinVolumeControl(QObject* parent):
    PlatformVolumeControl(parent),
    m_eventHandler(new WinVolumeEventHandler(GUID_QnobAudioEvent, this))
{
    /* WinVolumeEventHandler emits these signals from another thread. */
    connect(m_eventHandler.get(), &WinVolumeEventHandler::volumeChangedExternally, this, &WinVolumeControl::changedExternally);
    connect(m_eventHandler.get(), &WinVolumeEventHandler::defaultDeviceChanged, this, [&] {
        deinitializeDefaultDevice();
        initializeDefaultDevice();
    });

    if (!apicall(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_enumerator))))
        return;

    if (!apicall(m_enumerator->RegisterEndpointNotificationCallback(m_eventHandler.get())))
        return;

    initializeDefaultDevice();
}

WinVolumeControl::~WinVolumeControl() {
    if (m_enumerator)
        m_enumerator->UnregisterEndpointNotificationCallback(m_eventHandler.get());

    deinitializeDefaultDevice();
}

void WinVolumeControl::initializeDefaultDevice() {
    if (!m_enumerator)
        return;

    if (!apicall(m_enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, m_defaultDevice.mutablePtr())))
        return;

    if (!apicall(m_defaultDevice->Activate(m_volumeControl.staticId(), CLSCTX_INPROC_SERVER, nullptr, m_volumeControl.mutableVoidPtr())))
        return;

    if (!apicall(m_volumeControl->RegisterControlChangeNotify(m_eventHandler.get())))
        return;
}

void WinVolumeControl::deinitializeDefaultDevice() {
    if (m_volumeControl)
        if (!apicall(m_volumeControl->UnregisterControlChangeNotify(m_eventHandler.get())))
            return;
}

float WinVolumeControl::volume() const {
    if (!m_volumeControl)
        return NAN;

    float result;
    if (!apicall(m_volumeControl->GetMasterVolumeLevelScalar(&result)))
        return NAN;

    return result;
}

void WinVolumeControl::setVolume(float volume) {
    if (!m_volumeControl)
        return;

    if (!apicall(m_volumeControl->SetMasterVolumeLevelScalar(std::clamp(volume, 0.0f, 1.0f), &GUID_QnobAudioEvent)))
        return;
}

bool WinVolumeControl::isMuted() const {
    if (!m_volumeControl)
        return true;

    BOOL muted = FALSE;
    if(!apicall(m_volumeControl->GetMute(&muted)))
        return false;

    return muted;
}

void WinVolumeControl::setMuted(bool muted) {
    if (!m_volumeControl)
        return;

    if (!apicall(m_volumeControl->SetMute(muted, &GUID_QnobAudioEvent)))
        return;
}

