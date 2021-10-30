#include "win_shared_event_window.h"

#include <Windows.h>

#include <QtCore/QDebug>

#include <util/debug.h>

#include "win_error.h"

/**
 * This message is emitted whenever explorer is restarted. And when dpi changes.
 */
static UINT WM_TASKBARCREATED = 0;

/**
 * We're using this custom message instead of WM_DISPLAYCHANGE because apparently the latter one is emitted before
 * windows data structures for physical monitors are initialized. So if we call GetPhysicalMonitorsFromHMONITOR in
 * WM_DISPLAYCHANGE, it just returns physical monitors that are then destroyed 1s later, and all calls to them fail.
 *
 * Looking at messages in Spy++, this is the sequence:
 * - WM_DISPLAYCHANGE
 * - WM_DEVICECHANGE (x2)
 * - WM_SETTINGSCHANGE with SPI_SETWORKAREA
 * - UxdDisplayChangeMessage
 * - HotUnplugDetected (or HotplugDetected)
 * - Another UxdDisplayChangeMessage when unplugging a monitor
 *
 * So UxdDisplayChangeMessage seems to be a safe bet.
 */
static UINT WM_UXDDISPLAYCHANGE = 0;

WinSharedEventWindow::WinSharedEventWindow() {
    setTitle(lit("WinSharedEventWindow")); /* Mainly used for debugging with Spy++. */

    /* This magic here is needed because message-only windows don't receive broadcast messages, see
     * https://stackoverflow.com/questions/22570008/not-receiving-wm-displaychange */
    setFlag(Qt::FramelessWindowHint);
    setFlag(Qt::WindowTransparentForInput);
    setGeometry(0, 0, 0, 0);
    show(); /* This is where the underlying window is created. */
    hide();

    if (WM_TASKBARCREATED == 0)
        WM_TASKBARCREATED = RegisterWindowMessageW(L"TaskbarCreated");
    if (WM_UXDDISPLAYCHANGE == 0)
        WM_UXDDISPLAYCHANGE = RegisterWindowMessageW(L"UxdDisplayChangeMessage");

    apicall(ChangeWindowMessageFilterEx(reinterpret_cast<HWND>(winId()), WM_TASKBARCREATED, MSGFLT_ALLOW, nullptr));
}

bool WinSharedEventWindow::nativeEvent(const QByteArray& eventType, void* message, qintptr* result) {
    MSG* msg = static_cast<MSG*>(message);

    if (msg->message == WM_INPUT) {
        emit input(msg);
    } else if (msg->message == WM_HOTKEY) {
        emit hotkey(msg);
    } else if (msg->message == WM_UXDDISPLAYCHANGE) {
        xDebug("Received WM_UXDDISPLAYCHANGE");
        emit displayChange(msg);
    } else if (msg->message == WM_TASKBARCREATED) {
        xDebug("Received WM_TASKBARCREATED");
        emit taskbarCreated(msg);
    }

    return QWindow::nativeEvent(eventType, message, result);
}
