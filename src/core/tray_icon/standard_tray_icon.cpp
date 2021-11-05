#include "standard_tray_icon.h"

#include <cassert>
#include <chrono>

#include <QtCore/QTimer>
#include <QtGui/QCursor>

#include <core/setting/setting.h>
#include <platform/platform.h>

using namespace std::chrono_literals;

StandardTrayIcon::StandardTrayIcon(const QString& id, Setting* setting, PlatformStandardControl control) :
    Entity(id),
    m_control(platform()->createStandardControl(control, this)),
    m_setting(setting)
{
    platform()->wheelEventManager()->registerControl(m_control.get());

    connect(m_setting, &Setting::changed, this, &StandardTrayIcon::handleSettingChange);
}

StandardTrayIcon::~StandardTrayIcon() {
    platform()->wheelEventManager()->unregisterControl(m_control.get());
}

Setting* StandardTrayIcon::setting() const {
    return m_setting;
}

QObject* StandardTrayIcon::icon() const {
    return m_control.get();
}

void StandardTrayIcon::handleSettingChange() {
    if (m_control->geometry().contains(QCursor::pos())) {
        /* Quick & dirty workaround for the fact that tooltip is not updated when using mouse wheel over win audio icon.
         * It takes some time for updates to get delivered to explorer.exe, so we have to use a timer here.
         * Maybe a better solution exists. */

        auto update = [] { platform()->execute(WinUpdateCurrentToolTip); };
        QTimer::singleShot(10ms, this, update);
        QTimer::singleShot(50ms, this, update);
    }
}
