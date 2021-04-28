#include "win_thread_message_hook.h"

#include <Windows.h>

#include <QtCore/QDebug>

#include "win_error.h"

static WinThreadMessageHook* g_hookInstance = nullptr;
static HHOOK g_messageHook = 0;

LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam) {
    if (code < 0)
        return CallNextHookEx(g_messageHook, code, wParam, lParam);

    assert(g_hookInstance);

    CWPSTRUCT* a = reinterpret_cast<CWPSTRUCT*>(lParam);

    return CallNextHookEx(g_messageHook, code, wParam, lParam);
}

WinThreadMessageHook::WinThreadMessageHook() {
    g_hookInstance = this;
    g_messageHook = apicall(SetWindowsHookExW(WH_CALLWNDPROC, &CallWndProc, NULL, GetCurrentThreadId()));
}

WinThreadMessageHook::~WinThreadMessageHook() {
    // TODO
}
