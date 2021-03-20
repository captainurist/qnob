#include "win_standard_control.h"

#include <Windows.h>

#include "win_guids.h"
#include "win_error.h"

WinStandardControl::WinStandardControl(PlatformStandardControl control):
    m_control(control)
{}

WinStandardControl::~WinStandardControl() {}

QRect WinStandardControl::geometry() const {
    if (m_control != AudioTrayIcon)
        return QRect(); // TODO

    NOTIFYICONIDENTIFIER identifier;
    identifier.cbSize = sizeof(identifier);
    identifier.hWnd = 0;
    identifier.uID = 0;
    identifier.guidItem = GUID_TrayIconVolume;

    RECT location;
    if (!apicall(Shell_NotifyIconGetRect(&identifier, &location)))
        return QRect(); /* Icon not found. */

    return QRect(QPoint(location.left, location.top), QPoint(location.right, location.bottom));
}

PlatformStandardControl WinStandardControl::standardType() const {
    return m_control;
}
