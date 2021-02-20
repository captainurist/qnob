#pragma once

#include <QtGui/QKeySequence>

#include "platform_volume_control.h"
#include "platform_brightness_control.h"
#include "platform_shortcut_notifier.h"

class Platform {
public:
    virtual ~Platform() {}

    /**
     * \returns                         New volume control object.
     */
    virtual PlatformVolumeControl* createVolumeControl() const = 0;

    /**
     * \return                          Brightness control object. Never null.
     */
    virtual PlatformBrightnessControl* brightnessControl() const = 0;

    /**
     * \param shortcut                  Key sequence to create a global shortcut for.
     * \returns                         New shortcut notifier, or `nullptr` in case of an error.
     */
    virtual PlatformShortcutNotifier* createShortcutNotifier(const QKeySequence& shortcut) const = 0;

    static Platform* instance();
};

inline Platform* platform() {
    return Platform::instance();
}

