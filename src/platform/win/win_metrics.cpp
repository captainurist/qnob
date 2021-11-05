#include "win_metrics.h"

#include <Windows.h>
#include <ShellScalingApi.h>

#include <util/debug.h>

#include "win_error.h"
#include "win_shared_event_window.h"

static void checkProcessPerMonitorDpiAware() {
    static bool checked = false;
    if (checked)
        return;
    checked = true;

    PROCESS_DPI_AWARENESS awareness;
    if (!apicall(GetProcessDpiAwareness(NULL, &awareness)))
        return;

    if (awareness != PROCESS_PER_MONITOR_DPI_AWARE)
        xCritical("Per-monitor DPI awareness mode is required for the windows platform metrics code to work.");
}

static UINT getSystemTrayDpi() {
    constexpr UINT defaultDpi = USER_DEFAULT_SCREEN_DPI;

    /* In this function we're going the dpi-from-monitor route. Another option would've been to get system tray HWND
     * and then call GetDpiForWindow. */

    /* System tray always resides on the primary monitor. */
    HMONITOR monitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
    if (monitor == NULL)
        return defaultDpi; /* Should never happen. */

    UINT dpiX, dpiY;
    if (!apicall(GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY)))
        return defaultDpi;

    return dpiX;
}

static QSize getSystemTrayIconSize() {
    UINT dpi = getSystemTrayDpi();
    return QSize(GetSystemMetricsForDpi(SM_CXSMICON, dpi), GetSystemMetricsForDpi(SM_CYSMICON, dpi));
}

WinMetrics::WinMetrics(WinSharedEventWindow* eventWindow, QObject* parent):
    PlatformMetrics(parent)
{
    /* Taskbar is recreated when primary monitor dpi changes. */
    connect(eventWindow, &WinSharedEventWindow::taskbarCreated, this, &WinMetrics::updateTrayIconSize);
}

QSize WinMetrics::trayIconSize() const {
    checkProcessPerMonitorDpiAware();

    if (m_cachedTrayIconSize.isValid())
        return m_cachedTrayIconSize;

    return getSystemTrayIconSize();
}

void WinMetrics::updateTrayIconSize() {
    QSize newTrayIconSize = getSystemTrayIconSize();
    if (m_cachedTrayIconSize == newTrayIconSize)
        return;

    m_cachedTrayIconSize = newTrayIconSize;
    emit trayIconSizeChanged();
}
