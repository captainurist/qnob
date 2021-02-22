#pragma once

#include <QtCore/QObject>

class PlatformVolumeControl: public QObject {
    Q_OBJECT
public:
    /**
     * \returns                         Current system volume in [0, 1], or NAN in case of an error.
     */
    virtual float volume() const = 0;

    virtual void setVolume(float volume) = 0;

    virtual bool isMuted() const = 0;

    virtual void setMuted(bool muted) = 0;

signals:
    /**
     * Emitted whenever volume control state is changed externally.
     */
    void changedExternally();
};

