#ifndef PLATFORM_H
#define PLATFORM_H

#include "volume_control.h"

class Platform
{
public:
    virtual ~Platform() {}

    virtual VolumeControl* volumeControl() const = 0;

    static Platform* instance();
};

inline Platform* qPlatform() {
    return Platform::instance();
}

#endif // PLATFORM_H
