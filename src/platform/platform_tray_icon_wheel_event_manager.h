#pragma once

#include <QtCore/QObject>

class QSystemTrayIcon;

class PlatformTrayIconWheelEventManager : public QObject {
    Q_OBJECT
public:
    enum StandardTrayIcon {
        AudioTrayIcon
    };

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
     *                                  Re-registering the same standard icon is not allowed.
     * \param icon                      Object that will receive synthetic mouse wheel events.
     */
    virtual void registerStandardIcon(StandardTrayIcon standardIcon, QObject* icon) = 0;

    /**
     * \param standardIcon              Standard icon to unregister. It must have been previously registered.
     */
    virtual void unregisterStandardIcon(StandardTrayIcon standardIcon) = 0;
};
