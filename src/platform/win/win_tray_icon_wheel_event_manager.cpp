#include "win_tray_icon_wheel_event_manager.h"

#include <cassert>
#include <ranges>

#include <QtCore/QThread>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtGui/QWheelEvent>
#include <QtWidgets/QSystemTrayIcon>

#include "win_global_mouse_hook.h"
#include "win_guids.h"
#include "win_error.h"

static QRect QRectFromRECT(const RECT& rect) {
    return QRect(QPoint(rect.left, rect.top), QPoint(rect.right, rect.bottom));
}

static void sendSyntheticWheelEvent(QObject* target, const QRect& globalGeometry, UINT message, const MSLLHOOKSTRUCT& data) {
    int delta = GET_WHEEL_DELTA_WPARAM(data.mouseData);

    QWheelEvent event(
        QPoint(data.pt.x, data.pt.y) - globalGeometry.topLeft(),
        QPoint(data.pt.x, data.pt.y),
        QPoint(),
        message == WM_MOUSEWHEEL ? QPoint(0, delta) : QPoint(delta, 0),
        Qt::NoButton,
        Qt::NoModifier,
        Qt::NoScrollPhase,
        /*inverted=*/ false,
        Qt::MouseEventSynthesizedByApplication);

    qApp->notify(target, &event);
}

WinTrayIconWheelEventManager::WinTrayIconWheelEventManager(WinGlobalMouseHook* hook) {
    connect(hook, &WinGlobalMouseHook::messageHooked, this, &WinTrayIconWheelEventManager::processMessage);
}

WinTrayIconWheelEventManager::~WinTrayIconWheelEventManager() {}

void WinTrayIconWheelEventManager::registerTrayIcon(QSystemTrayIcon* icon) {
    assert(!m_icons.contains(icon));
    m_icons.insert(icon);
}

void WinTrayIconWheelEventManager::unregisterTrayIcon(QSystemTrayIcon* icon) {
    assert(m_icons.contains(icon));
    m_icons.erase(icon);
}

void WinTrayIconWheelEventManager::registerStandardIcon(PlatformStandardTrayIcon standardIcon, QObject* icon) {
    assert(!m_objectsByStandardIcon[standardIcon].contains(icon));
    m_objectsByStandardIcon[standardIcon].insert(icon);
}

void WinTrayIconWheelEventManager::unregisterStandardIcon(PlatformStandardTrayIcon standardIcon, QObject* icon) {
    assert(m_objectsByStandardIcon[standardIcon].contains(icon));
    m_objectsByStandardIcon[standardIcon].erase(icon);
}

void WinTrayIconWheelEventManager::processMessage(UINT message, const MSLLHOOKSTRUCT& data) {
    assert(message == WM_MOUSEWHEEL || message == WM_MOUSEHWHEEL);

    HWND window = WindowFromPoint(data.pt);
    if (window == NULL)
        return; /* No window here. */

    HWND parentWindow = GetAncestor(window, GA_ROOT);
    if (parentWindow == NULL)
        return; /* Should never happen, we just check to feel safe. */

    WCHAR classNameBuffer[260]; /* Max length is 256, see docs for WNDCLASSW structure. */
    int classNameSize = GetClassNameW(parentWindow, classNameBuffer, std::size(classNameBuffer));
    QString className = QString::fromWCharArray(classNameBuffer, classNameSize);
    if (className != lit("Shell_TrayWnd") && className != lit("NotifyIconOverflowWindow"))
        return; /* Not scrolling over a tray window. */

    QPoint globalPoint(data.pt.x, data.pt.y);
    for (QSystemTrayIcon* icon : m_icons) {
        QRect globalGeometry = icon->geometry();

        if (globalGeometry.contains(globalPoint)) {
            sendSyntheticWheelEvent(icon, globalGeometry, message, data);
            return;
        }
    }

    for (const auto& [standardIcon, objects] : m_objectsByStandardIcon) {
        if(objects.empty())
            continue;

        assert(standardIcon == AudioTrayIcon);

        NOTIFYICONIDENTIFIER identifier;
        identifier.cbSize = sizeof(identifier);
        identifier.hWnd = 0;
        identifier.uID = 0;
        identifier.guidItem = GUID_TrayIconVolume;

        RECT location;
        if (!apicall(Shell_NotifyIconGetRect(&identifier, &location)))
            continue; /* Icon not found. */
        QRect globalGeometry = QRectFromRECT(location);

        if (globalGeometry.contains(globalPoint)) {
            for(QObject* object: objects)
                sendSyntheticWheelEvent(object, globalGeometry, message, data);
            return;
        }
    }
}
