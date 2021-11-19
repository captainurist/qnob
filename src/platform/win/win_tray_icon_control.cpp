#include "win_tray_icon_control.h"

#include <Windows.h>

#include <util/debug.h>

#include "win_error.h"

WinTrayIconControl::WinTrayIconControl(const GUID& guid, QObject* parent):
    PlatformControl(parent),
    m_guid(guid)
{}

WinTrayIconControl::~WinTrayIconControl() {}

QRect WinTrayIconControl::geometry() const {
    NOTIFYICONIDENTIFIER identifier;
    identifier.cbSize = sizeof(identifier);
    identifier.hWnd = 0;
    identifier.uID = 0;
    identifier.guidItem = m_guid;

    RECT location;
    if (!apicall(Shell_NotifyIconGetRect(&identifier, &location)))
        return QRect(); /* Icon not found. */

    return QRect(QPoint(location.left, location.top), QPoint(location.right, location.bottom));
}

bool WinTrayIconControl::isMinimized() const {
    return false;
}

void WinTrayIconControl::setMinimized(bool) {
    xWarning("Can't control minimized state of tray icons.");
}
