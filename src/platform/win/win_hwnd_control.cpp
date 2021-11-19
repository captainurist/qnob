#include "win_hwnd_control.h"

#include "win_error.h"

WinHwndControl::WinHwndControl(HWND hwnd, QObject* parent) :
    PlatformControl(parent),
    m_hwnd(hwnd)
{}

WinHwndControl::~WinHwndControl() {}

QRect WinHwndControl::geometry() const {
    RECT rect;
    if (!apicall(GetWindowRect(m_hwnd, &rect)))
        return QRect();

    return QRect(QPoint(rect.left, rect.top), QPoint(rect.right, rect.bottom));
}

bool WinHwndControl::isMinimized() const {
    return IsIconic(m_hwnd);
}

void WinHwndControl::setMinimized(bool minimized) {
    apicall(ShowWindow(m_hwnd, minimized ? SW_MINIMIZE : SW_RESTORE));
}
