#pragma once

#include <QtGui/QKeySequence>

#include "platform_volume_control.h"
#include "platform_shortcut_notifier.h"

class Platform {
public:
    virtual ~Platform() {}

    virtual PlatformVolumeControl* volumeControl() const = 0;
    virtual PlatformShortcutNotifier* createShortcutNotifier(const QKeySequence& shortcut) const = 0;

    static Platform* instance();
};

inline Platform* qPlatform() {
    return Platform::instance();
}

