#include "win_platform.h"

#include <Windows.h>

#include <QtCore/QScopeGuard>

#include <util/debug.h>

#include "com.h"
#include "win_error.h"
#include "win_volume_control.h"
#include "win_monitor_manager.h"
#include "win_shortcut_manager.h"
#include "win_wheel_event_manager.h"
#include "win_metrics.h"
#include "win_functions.h"
#include "win_shared_event_window.h"
#include "win_tray_icon_control.h"
#include "win_hwnd_control.h"
#include "win_guids.h"

WinPlatform::WinPlatform(QObject* parent):
    Platform(parent)
{
    m_com.reset(new Com());
    m_eventWindow.reset(new WinSharedEventWindow());
    m_volumeControl.reset(new WinVolumeControl(this));
    m_monitorManager.reset(new WinMonitorManager(m_eventWindow.get(), this));
    m_shortcutManager.reset(new WinShortcutManager(m_eventWindow.get(), this));
    m_wheelEventManager.reset(new WinWheelEventManager(m_eventWindow.get(), this));
    m_metrics.reset(new WinMetrics(m_eventWindow.get(), this));
    m_functions.reset(new WinFunctions(this));
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

PlatformFunctions* WinPlatform::functions() const {
    return m_functions.get();
}

std::unique_ptr<PlatformControl> WinPlatform::createStandardControl(PlatformStandardControl control, QObject* parent) const {
    switch (control) {
    case AudioTrayIconControl:
        return std::make_unique<WinTrayIconControl>(GUID_TrayIconVolume, parent);
    case NativeOsdControl:
        return createNativeOsdControl(parent);
    default:
        return nullptr;
    }
}

std::unique_ptr<PlatformControl> WinPlatform::createNativeOsdControl(QObject* parent) const {
    HWND resultWindow = NULL;
    HWND currentWindow = NULL;
    size_t count = 0;

    while ((currentWindow = FindWindowExW(NULL, currentWindow, L"NativeHWNDHost", L"")) != NULL) {
        if (FindWindowExW(currentWindow, NULL, L"DirectUIHWND", L"") == 0)
            continue; /* Doesn't have the child window that we need. */

        DWORD pid;
        DWORD threadId = GetWindowThreadProcessId(currentWindow, &pid);
        std::optional<HANDLE> process = apicall(OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid));
        if (!process)
            continue;
        auto cleanup = QScopeGuard([&] { apicall(CloseHandle(*process)); });

        WCHAR path[MAX_PATH];
        DWORD size = MAX_PATH;
        if (!apicall(QueryFullProcessImageNameW(*process, 0, path, &size)))
            continue;

        QString normalizedPath = QString::fromWCharArray(path).toLower().replace(QLatin1Char('\\'), QLatin1Char('/'));
        if (!normalizedPath.endsWith(lit("/explorer.exe")))
            continue;

        count++;
        resultWindow = currentWindow;
    }

    if (count > 1) {
        xWarning("Something's off, found {} native OSD controls", count);
        return nullptr;
    } else if (count == 1) {
        return std::make_unique<WinHwndControl>(resultWindow, parent);
    } else {
        return nullptr;
    }
}

Platform* createPlatform() {
    return new WinPlatform(nullptr);
}
