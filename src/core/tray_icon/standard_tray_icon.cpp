#include "standard_tray_icon.h"

#include <cassert>
#include <chrono>

#include <QtCore/QTimer>

#include <core/setting/setting.h>
#include <platform/platform.h>

using namespace std::chrono_literals;

StandardTrayIcon::StandardTrayIcon(const QString& id, Setting* setting, PlatformStandardTrayIcon standardIcon) :
    Entity(id),
    m_setting(setting),
    m_standardIcon(standardIcon)
{
    platform()->trayIconWheelEventManager()->registerStandardIcon(m_standardIcon, this);

    connect(m_setting, &Setting::changed, this, &StandardTrayIcon::handleSettingChange);
}

StandardTrayIcon::~StandardTrayIcon() {
    platform()->trayIconWheelEventManager()->unregisterStandardIcon(m_standardIcon, this);
}

Setting* StandardTrayIcon::setting() const {
    return m_setting;
}

QObject* StandardTrayIcon::icon() const {
    return const_cast<StandardTrayIcon*>(this);
}

void StandardTrayIcon::handleSettingChange() {
    if (m_standardIcon == AudioTrayIcon) {
        /* Quick & dirty workaround for the fact that tooltip is not updated when using mouse wheel over win audio icon.
         * It takes some time for updates to get delivered to explorer.exe, so we have to use a timer here.
         * Maybe a better solution exists. */

        auto update = [] { platform()->execute(WinUpdateCurrentToolTip); };
        QTimer::singleShot(10ms, this, update);
        QTimer::singleShot(50ms, this, update);
    }
}
