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
#include "platform_functions.h"

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
    Platform(QObject* parent) : QObject(parent) {}

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
     * \returns                         Platform functions object.
     */
    virtual PlatformFunctions* functions() const = 0;

    /**
     * \param control                   Standard control.
     * \returns                         Platform control object for the provided standard control.
     *                                  Might return `nullptr` if the requested standard control doesn't exist.
     */
    virtual std::unique_ptr<PlatformControl> createStandardControl(PlatformStandardControl control, QObject* parent) const = 0;
};

/**
 * \returns                             Global platform instance.
 */
Platform* platform();
