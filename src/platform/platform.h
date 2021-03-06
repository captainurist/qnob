#pragma once

#include <QtCore/QObject>
#include <QtCore/QVariant>

#include "platform_volume_control.h"
#include "platform_shortcut_notifier.h"
#include "platform_monitor_manager.h"
#include "platform_wheel_event_manager.h"
#include "platform_shortcut_manager.h"
#include "platform_control.h"
#include "platform_metrics.h"

/**
 * Platform abstraction class.
 *
 * Instantiate `PlatformInitializer` in your code first, then you'll be able to use platform global instance.
 *
 * Some guidelines for platform classes:
 * - They are pretty low level, convenience classes are to be added on top.
 * - They don't throw exceptions.
 */
class Platform : public QObject {
    Q_OBJECT
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
     * \returns                         Shortcut manager object.
     */
    virtual PlatformShortcutManager* shortcutManager() const = 0;

    /**
     * \returns                         Tray icon wheel event manager object.
     */
    virtual PlatformWheelEventManager* wheelEventManager() const = 0;

    /**
     * \returns                         Metrics object.
     */
    virtual PlatformMetrics* metrics() const = 0;

    /**
     * \param control                   Standard control.
     * \returns                         Platform control object for the provided standard control.
     */
    virtual PlatformControl* createStandardControl(PlatformStandardControl control) const = 0;

    /**
     * Runs a platform-specific function.
     *
     * \param function                  Platform function to run.
     * \param arg0                      First argument, if any. Argument type mismatches result in assertions.
     * \returns                         Result, if any.
     */
    virtual QVariant execute(PlatformFunction function, QVariant arg0 = QVariant()) = 0;
};

/**
 * \returns                             Global platform instance.
 */
Platform* platform();
