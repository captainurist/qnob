#pragma once

class PlatformVolumeControl {
public:
    virtual ~PlatformVolumeControl() {}

    virtual float volume() const = 0;
    virtual void setVolume(float volume) = 0;
    virtual bool isMuted() const = 0;
    virtual void setMuted(bool muted) = 0;

    // TODO
    // volumeChanged
};

