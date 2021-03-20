#pragma once

#include <QtCore/QObject>

#include "platform_enums.h"

class QSystemTrayIcon;
class PlatformControl;

class PlatformWheelEventManager : public QObject {
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
     * \param control                   Control to register. Re-registration is not allowed.
     */
    virtual void registerControl(PlatformControl* control) = 0;

    /**
     * \param control                   Control to unregister. It must have been previously registered.
     */
    virtual void unregisterControl(PlatformControl* control) = 0;
};
