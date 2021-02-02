#pragma once

#include "platform/platform_volume_control.h"

#include "com_ptr.h"

struct IMMDeviceEnumerator;
struct IMMDevice;
struct IAudioEndpointVolume;

class WinVolumeControl: public PlatformVolumeControl {
public:
    WinVolumeControl();
    virtual ~WinVolumeControl();

    virtual float volume() const override;
    virtual void setVolume(float volume) override;
    virtual bool isMuted() const override;
    virtual void setMuted(bool muted) override;

private:
    ComPtr<IMMDeviceEnumerator> m_enumerator;
    ComPtr<IMMDevice> m_defaultDevice;
    ComPtr<IAudioEndpointVolume> m_volumeControl;
};

