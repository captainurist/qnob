#include "win_shared_event_window.h"

#include <Windows.h>

#include <QtCore/QDebug>

#include "win_error.h"

static UINT WM_TASKBARCREATED = 0;

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

    apicall(ChangeWindowMessageFilterEx(reinterpret_cast<HWND>(winId()), WM_TASKBARCREATED, MSGFLT_ALLOW, nullptr));
}

bool WinSharedEventWindow::nativeEvent(const QByteArray& eventType, void* message, qintptr* result) {
    MSG* msg = static_cast<MSG*>(message);

    if (msg->message == WM_INPUT) {
        emit input(msg);
    } else if (msg->message == WM_HOTKEY) {
        emit hotkey(msg);
    } else if (msg->message == WM_DISPLAYCHANGE) {
        emit displayChange(msg);
    } else if (msg->message == WM_TASKBARCREATED) {
        emit taskbarCreated(msg);
    }

    return QWindow::nativeEvent(eventType, message, result);
}
