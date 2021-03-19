#include "standard_tray_icon.h"

#include <cassert>

#include <platform/platform.h>

StandardTrayIcon::StandardTrayIcon(const QString& id, PlatformStandardTrayIcon standardIcon) :
    Entity(id),
    m_standardIcon(standardIcon)
{
    platform()->trayIconWheelEventManager()->registerStandardIcon(m_standardIcon, this);
}

StandardTrayIcon::~StandardTrayIcon() {
    platform()->trayIconWheelEventManager()->unregisterStandardIcon(m_standardIcon, this);
}

QObject* StandardTrayIcon::icon() const {
    return const_cast<StandardTrayIcon*>(this);
}
