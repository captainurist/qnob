#include "win_global_mouse_hook.h"

#include <Windows.h>

#include <cassert>

#include "win_error.h"

WinGlobalMouseHook* g_hookInstance = nullptr;
HHOOK g_mouseHook = 0;

LRESULT CALLBACK LowLevelMouseProc(int code, WPARAM wParam, LPARAM lParam) {
    if (code < 0)
        return CallNextHookEx(g_mouseHook, code, wParam, lParam);

    assert(g_hookInstance);
    if (wParam == WM_MOUSEWHEEL)
        emit g_hookInstance->messageHooked(wParam, *reinterpret_cast<MSLLHOOKSTRUCT*>(lParam));

    return CallNextHookEx(g_mouseHook, code, wParam, lParam);
}

WinGlobalMouseHook::WinGlobalMouseHook() {
    assert(g_hookInstance == nullptr);
    g_hookInstance = this;

    // TODO: check return value
    g_mouseHook = SetWindowsHookExW(WH_MOUSE_LL, &LowLevelMouseProc, NULL, 0);
}

WinGlobalMouseHook::~WinGlobalMouseHook() {
    if(g_mouseHook)
        succeeded(UnhookWindowsHookEx(g_mouseHook));

    assert(g_hookInstance == this);
    g_hookInstance = nullptr;
}
