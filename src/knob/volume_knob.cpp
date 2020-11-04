#include "volume_knob.h"

#include <combaseapi.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#include <com/com_ptr.h>

static const GUID GUID_QnobAudioEvent = {0x3bd43a3e, 0xd585, 0x4354, {0xcb, 0xf3, 0x55, 0xba, 0x11, 0xfc, 0x84, 0x12}};
static const CLSID CLSID_MMDeviceEnumerator = {0xBCDE0395, 0xE52F, 0x467C, {0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E}};

class VolumeKnobPrivate {
public:
    void init() {
        if (!SUCCEEDED(CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&enumerator))))
            return;

        if (!SUCCEEDED(enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, defaultDevice.mutablePtr())))
            return;

        if (!SUCCEEDED(defaultDevice->Activate(volumeControl.staticId(), CLSCTX_INPROC_SERVER, nullptr, volumeControl.mutableVoidPtr())))
            return;
    }

    int value() const {
        if (!volumeControl)
            return 0;

        float result;
        if (!SUCCEEDED(volumeControl->GetMasterVolumeLevelScalar(&result)))
            return 0;

        return result * Knob::MaxValue;
    }

    void setValue(int value) {
        if (!volumeControl)
            return;
        
        volumeControl->SetMasterVolumeLevelScalar(qBound(Knob::MinValue, value, Knob::MaxValue) * 1.0f / Knob::MaxValue, &GUID_QnobAudioEvent);
    }

    bool enabled() const {
        if (!volumeControl)
            return false;

        BOOL muted = FALSE;
        if(!SUCCEEDED(volumeControl->GetMute(&muted)))
            return false;

        return !muted;
    }

    void setEnabled(bool value) {
        if (!volumeControl)
            return;
        
        volumeControl->SetMute(!value, &GUID_QnobAudioEvent);
    }

public:
    ComPtr<IMMDeviceEnumerator> enumerator;
    ComPtr<IMMDevice> defaultDevice;
    ComPtr<IAudioEndpointVolume> volumeControl;
    VolumeKnob* q = nullptr;
};

VolumeKnob::VolumeKnob(QObject* parent) : 
    Knob(parent),
    d(new VolumeKnobPrivate)
{
    d->q = this;
    d->init();
}

VolumeKnob::~VolumeKnob() 
{}

QByteArray VolumeKnob::id() const {
    return "volume";
}

int VolumeKnob::value() const {
    return d->value();
}

void VolumeKnob::setValue(int value) {
    d->setValue(value);
}

bool VolumeKnob::enabled() const {
    return d->enabled();
}

void VolumeKnob::setEnabled(bool value) {
    d->setEnabled(value);
}

