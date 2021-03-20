#include "win_platform.h"

#include <Windows.h>
#include <CommCtrl.h>

#include <cassert>

#include <QtCore/QThread>

#include "com.h"
#include "win_volume_control.h"
#include "win_monitor_manager.h"
#include "win_shortcut_manager.h"
#include "win_wheel_event_manager.h"
#include "win_global_mouse_hook.h"
#include "win_standard_control.h"

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
    m_trayIconWheelEventManager.reset(new WinWheelEventManager(m_hook));
    m_shortcutManager.reset(new WinShortcutManager());

    /* Note that hooks are disabled by default, we don't call hookChangeRequested(true) here. */
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

PlatformWheelEventManager* WinPlatform::trayIconWheelEventManager() const {
    return m_trayIconWheelEventManager.get();
}

PlatformShortcutNotifier* WinPlatform::createShortcutNotifier(const QKeyCombination& shortcut) const {
    return m_shortcutManager->createShortcutNotifier(shortcut);
}

PlatformControl* WinPlatform::createStandardControl(PlatformStandardControl control) const {
    return new WinStandardControl(control);
}

void WinPlatform::execute(PlatformFunction function) {
    switch (function) 	{
    case WinToggleHooks:
        emit hookChangeRequested(!m_hook->isEnabled());
        break;
    default:
        assert(function == WinUpdateCurrentToolTip);
    case WinUpdateCurrentToolTip:
        updateCurrentToolTip();
        break;
    }
}

void WinPlatform::updateCurrentToolTip() const {
    HWND hwnd = NULL;

    while (hwnd = FindWindowExW(NULL, hwnd, L"tooltips_class32", NULL)) {
        if (!IsWindowVisible(hwnd))
            continue;

        PostMessageW(hwnd, TTM_UPDATE, 0, 0);
        return;
    }
}

Platform* createPlatform() {
    return new WinPlatform();
}
