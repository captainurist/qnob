#pragma once

#include <QtCore/QObject>

#include "platform_enums.h"

class QSystemTrayIcon;

class PlatformTrayIconWheelEventManager : public QObject {
    Q_OBJECT
public:
    /**
     * \param icon                      Icon to register. Re-registration is not allowed.
     */
    virtual void registerTrayIcon(QSystemTrayIcon* icon) = 0;

    /**
     * \param icon                      Icon to unregister. It must have been previously registered.
     */
    virtual void unregisterTrayIcon(QSystemTrayIcon* icon) = 0;

    /**
     * \param standardIcon              Standard icon to get notifications for.
     * \param icon                      Object that will receive synthetic mouse wheel events.
     */
    virtual void registerStandardIcon(PlatformStandardTrayIcon standardIcon, QObject* icon) = 0;

    /**
     * \param standardIcon              Standard icon.
     * \param icon                      Notification object.
     */
    virtual void unregisterStandardIcon(PlatformStandardTrayIcon standardIcon, QObject* icon) = 0;
};
