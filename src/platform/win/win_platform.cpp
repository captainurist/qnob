#include "win_platform.h"

#include <Windows.h>
#include <CommCtrl.h>
#include <io.h>
#include <fcntl.h>

#include <cassert>

#include <QtCore/QThread>
#include <QtCore/QVariant>

#include "com.h"
#include "win_error.h"
#include "win_volume_control.h"
#include "win_monitor_manager.h"
#include "win_shortcut_manager.h"
#include "win_wheel_event_manager.h"
#include "win_metrics.h"
#include "win_shared_event_window.h"
#include "win_standard_control.h"

WinPlatform::WinPlatform() {
    m_com.reset(new Com());
    m_eventWindow.reset(new WinSharedEventWindow());
    m_volumeControl.reset(new WinVolumeControl());
    m_monitorManager.reset(new WinMonitorManager(m_eventWindow.get()));
    m_shortcutManager.reset(new WinShortcutManager(m_eventWindow.get()));
    m_wheelEventManager.reset(new WinWheelEventManager(m_eventWindow.get()));
    m_metrics.reset(new WinMetrics(m_eventWindow.get()));
}

WinPlatform::~WinPlatform() {}

PlatformVolumeControl* WinPlatform::volumeControl() const {
    return m_volumeControl.get();
}

PlatformMonitorManager* WinPlatform::monitorManager() const {
    return m_monitorManager.get();
}

PlatformShortcutManager* WinPlatform::shortcutManager() const {
    return m_shortcutManager.get();
}

PlatformWheelEventManager* WinPlatform::wheelEventManager() const {
    return m_wheelEventManager.get();
}

PlatformMetrics* WinPlatform::metrics() const {
    return m_metrics.get();
}

PlatformControl* WinPlatform::createStandardControl(PlatformStandardControl control) const {
    return new WinStandardControl(control);
}

QVariant WinPlatform::execute(PlatformFunction function, QVariant arg0) {
    switch (function) {
    case GetConsoleSize:
        if (QSize size = getConsoleSize(); size.isValid())
            return size;
        return QVariant();
    case WinUpdateCurrentToolTip:
        updateCurrentToolTip();
        return QVariant();
    case WinHideConsole:
        hideConsole();
        return QVariant();
    default:
        assert(false);
        return QVariant();
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

QSize WinPlatform::getConsoleSize() const {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (apicall(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))) {
        return QSize(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
    } else {
        return QSize();
    }
}

void WinPlatform::hideConsole() const {
    if (HWND console = GetConsoleWindow(); console != NULL)
        ShowWindow(console, SW_HIDE);
}

Platform* createPlatform() {
    return new WinPlatform();
}
