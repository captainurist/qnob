#include "win_platform.h"

#include <QtCore/QThread>

#include "com.h"
#include "win_volume_control.h"
#include "win_monitor_manager.h"
#include "win_shortcut_manager.h"
#include "win_tray_icon_wheel_event_manager.h"
#include "win_global_mouse_hook.h"

WinPlatform::WinPlatform() {
    m_com.reset(new Com());

    m_hookThread.reset(new QThread());
    m_hookThread->setObjectName(lit("GlobalMouseHookThread"));
    m_hookThread->start(QThread::HighestPriority); /* We want the hook proc to always return FAST, thus the custom priority here. */

    m_hook = new WinGlobalMouseHook();
    m_hook->moveToThread(m_hookThread.get());
    connect(m_hookThread.get(), &QThread::finished, m_hook, &QObject::deleteLater);
    connect(this, &WinPlatform::hookChangeRequested, m_hook, &WinGlobalMouseHook::setEnabled);

    m_volumeControl.reset(new WinVolumeControl());
    m_monitorManager.reset(new WinMonitorManager());
    m_trayIconWheelEventManager.reset(new WinTrayIconWheelEventManager(m_hook));
    m_shortcutDispatcher.reset(new WinShortcutManager());

    /* Enable the hook. Gotta do this through the event loop so that the registration is done on the hook thread,
     * as this is where the hook handler will be called. */
    emit hookChangeRequested(true);
}

WinPlatform::~WinPlatform() {
    m_hookThread->exit();
    m_hookThread->wait();
}

PlatformVolumeControl* WinPlatform::volumeControl() const {
    return m_volumeControl.get();
}

PlatformMonitorManager* WinPlatform::monitorManager() const {
    return m_monitorManager.get();
}

PlatformTrayIconWheelEventManager* WinPlatform::trayIconWheelEventManager() const {
    return m_trayIconWheelEventManager.get();
}

PlatformShortcutNotifier* WinPlatform::createShortcutNotifier(const QKeySequence& shortcut) const {
    return m_shortcutDispatcher->createShortcutNotifier(shortcut);
}

Platform* createPlatform() {
    return new WinPlatform();
}

bool WinPlatform::hooksEnabled() const {
    return m_hook->isEnabled();
}

void WinPlatform::setHooksEnabled(bool enabled) {
    emit hookChangeRequested(enabled);
}
