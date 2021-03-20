#include "win_global_mouse_hook.h"

#include <Windows.h>

#include <cassert>

#include "win_error.h"

static WinGlobalMouseHook* g_hookInstance = nullptr;
static HHOOK g_mouseHook = 0;

LRESULT CALLBACK LowLevelMouseProc(int code, WPARAM wParam, LPARAM lParam) {
    if (code < 0)
        return CallNextHookEx(g_mouseHook, code, wParam, lParam);

    assert(g_hookInstance);
    if (wParam == WM_MOUSEWHEEL || wParam == WM_MOUSEHWHEEL)
        emit g_hookInstance->messageHooked(wParam, *reinterpret_cast<MSLLHOOKSTRUCT*>(lParam));

    return CallNextHookEx(g_mouseHook, code, wParam, lParam);
}

WinGlobalMouseHook::WinGlobalMouseHook() {
    assert(g_hookInstance == nullptr);
    g_hookInstance = this;
}

WinGlobalMouseHook::~WinGlobalMouseHook() {
    setEnabled(false);

    assert(g_hookInstance == this);
    g_hookInstance = nullptr;
}

bool WinGlobalMouseHook::isEnabled() {
    return m_enabled;
}

void WinGlobalMouseHook::setEnabled(bool enabled) {
    if (m_enabled == enabled)
        return;

    if (m_enabled && !enabled) {
        if (g_mouseHook)
            apicall(UnhookWindowsHookEx(g_mouseHook));
    }

    if (!m_enabled && enabled) {
        g_mouseHook = apicall(SetWindowsHookExW(WH_MOUSE_LL, &LowLevelMouseProc, NULL, 0));
    }

    m_enabled = enabled;
}
