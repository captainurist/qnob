#include "win_platform.h"

#include "com.h"
#include "win_volume_control.h"
#include "win_monitor_manager.h"
#include "win_shortcut_manager.h"
#include "win_tray_icon_wheel_event_manager.h"

WinPlatform::WinPlatform() {
    m_com.reset(new Com());
    m_volumeControl.reset(new WinVolumeControl());
    m_monitorManager.reset(new WinMonitorManager());
    m_trayIconWheelEventManager.reset(new WinTrayIconWheelEventManager());
    m_shortcutDispatcher.reset(new WinShortcutManager());
}

WinPlatform::~WinPlatform() {
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
