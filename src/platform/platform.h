#pragma once

#include <QtGui/QKeySequence>

#include "platform_volume_control.h"
#include "platform_shortcut_notifier.h"
#include "platform_monitor_manager.h"
#include "platform_tray_icon_wheel_event_manager.h"

/**
 * Platform abstraction class.
 *
 * Instantiate `PlatformInitializer` in your code first, then you'll be able to use platform global instance.
 *
 * Some guidelines for platform classes:
 * - They are pretty low level, convenience classes are to be added on top.
 * - They don't throw exceptions.
 */
class Platform {
public:
    virtual ~Platform() {}

    /**
     * \returns                         Volume control object.
     */
    virtual PlatformVolumeControl* volumeControl() const = 0;

    /**
     * \returns                         Monitor manager object.
     */
    virtual PlatformMonitorManager* monitorManager() const = 0;

    /**
     * \returns                         Tray icon wheel event manager object.
     */
    virtual PlatformTrayIconWheelEventManager* trayIconWheelEventManager() const = 0;

    /**
     * \param shortcut                  Key sequence to create a global shortcut for.
     * \returns                         New shortcut notifier, or `nullptr` in case of an error.
     */
    virtual PlatformShortcutNotifier* createShortcutNotifier(const QKeySequence& shortcut) const = 0;
};

/**
 * \returns                             Global platform instance.
 */
Platform* platform();
