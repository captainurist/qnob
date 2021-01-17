#pragma once

#include "platform/platform.h"

#include <QtCore/QScopedPointer>

class Com;
class WinVolumeControl;

class WinPlatform: public Platform {
public:
    WinPlatform();
    virtual ~WinPlatform();

    virtual VolumeControl* volumeControl() const override;

private:
    QScopedPointer<Com> m_com;
    QScopedPointer<WinVolumeControl> m_volumeControl;
};

