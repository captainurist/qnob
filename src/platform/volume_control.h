#ifndef VOLUME_CONTROL_H
#define VOLUME_CONTROL_H

class VolumeControl {
public:
    virtual ~VolumeControl() {}

    virtual float volume() const = 0;
    virtual void setVolume(float volume) = 0;
    virtual bool isMuted() const = 0;
    virtual void setMuted(bool muted) = 0;

    // TODO
    // volumeChanged
};

#endif // VOLUME_CONTROL_H
