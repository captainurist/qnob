#pragma once

#include <QtCore/QObject>

class QSystemTrayIcon;

class PlatformTrayIconWheelEventManager : public QObject {
    Q_OBJECT
public:
    /**
     * \param icon                      Icon to register. Must not be null.
     */
    virtual void registerTrayIcon(QSystemTrayIcon* icon) = 0;

    /**
     * \param icon                      Icon to register. Must be previously registered.
     */
    virtual void unregisterTrayIcon(QSystemTrayIcon* icon) = 0;
};
