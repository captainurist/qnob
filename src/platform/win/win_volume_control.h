#pragma once

#include <platform/platform_volume_control.h>

#include "com_ptr.h"

struct IMMDeviceEnumerator;
struct IMMDevice;
struct IAudioEndpointVolume;
class WinVolumeEventHandler;

class WinVolumeControl: public PlatformVolumeControl {
public:
    WinVolumeControl(QObject* parent);
    virtual ~WinVolumeControl();

    virtual float volume() const override;
    virtual void setVolume(float volume) override;
    virtual bool isMuted() const override;
    virtual void setMuted(bool muted) override;

private:
    void initializeDefaultDevice();
    void deinitializeDefaultDevice();

private:
    ComPtr<IMMDeviceEnumerator> m_enumerator;
    ComPtr<IMMDevice> m_defaultDevice;
    ComPtr<IAudioEndpointVolume> m_volumeControl;
    ComPtr<WinVolumeEventHandler> m_eventHandler;
};
