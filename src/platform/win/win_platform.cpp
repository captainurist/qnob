#include "win_platform.h"

#include "com.h"
#include "win_volume_control.h"
#include "win_monitor_manager.h"
#include "win_shortcut_dispatcher.h"

WinPlatform::WinPlatform() {
    m_com.reset(new Com());
    m_shortcutDispatcher.reset(new WinShortcutDispatcher());
}

WinPlatform::~WinPlatform() {
}

PlatformVolumeControl* WinPlatform::createVolumeControl() const {
    return new WinVolumeControl();
}

PlatformMonitorManager* WinPlatform::createMonitorManager() const {
    return new WinMonitorManager();
}

PlatformShortcutNotifier* WinPlatform::createShortcutNotifier(const QKeySequence& shortcut) const {
    return m_shortcutDispatcher->createShortcutNotifier(shortcut);
}

Platform* createPlatform() {
    return new WinPlatform();
}
