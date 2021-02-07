#pragma once

#include <QtCore/QObject>

class PlatformVolumeControl: public QObject {
    Q_OBJECT
public:
    virtual ~PlatformVolumeControl() {}

    virtual float volume() const = 0;
    virtual void setVolume(float volume) = 0;
    virtual bool isMuted() const = 0;
    virtual void setMuted(bool muted) = 0;

signals:
    void stateChanged();
};

