#include "win_wheel_event_manager.h"

#include <cassert>
#include <ranges>

#include <QtCore/QThread>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtGui/QWheelEvent>
#include <QtWidgets/QSystemTrayIcon>

#include <platform/platform_control.h>

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

WinWheelEventManager::WinWheelEventManager(WinGlobalMouseHook* hook) {
    connect(hook, &WinGlobalMouseHook::messageHooked, this, &WinWheelEventManager::processMessage);
}

WinWheelEventManager::~WinWheelEventManager() {}

void WinWheelEventManager::registerTrayIcon(QSystemTrayIcon* icon) {
    assert(!m_icons.contains(icon));
    m_icons.insert(icon);
}

void WinWheelEventManager::unregisterTrayIcon(QSystemTrayIcon* icon) {
    assert(m_icons.contains(icon));
    m_icons.erase(icon);
}

void WinWheelEventManager::registerControl(PlatformControl* control) {
    assert(!m_controls.contains(control));
    m_controls.insert(control);
}

void WinWheelEventManager::unregisterControl(PlatformControl* control) {
    assert(m_controls.contains(control));
    m_controls.erase(control);
}

void WinWheelEventManager::processMessage(UINT message, const MSLLHOOKSTRUCT& data) {
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

    for (PlatformControl* control : m_controls) {
        QRect globalGeometry = control->geometry();

        if (globalGeometry.contains(globalPoint)) {
            sendSyntheticWheelEvent(control, globalGeometry, message, data);
            /* Can't return here, can have duplicates in standard controls. */
        }
    }
}
