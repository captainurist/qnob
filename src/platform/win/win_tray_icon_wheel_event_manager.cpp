#include "win_tray_icon_wheel_event_manager.h"

#include <cassert>

#include <QtCore/QThread>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtGui/QWheelEvent>
#include <QtWidgets/QSystemTrayIcon>

#include "win_global_mouse_hook.h"

WinTrayIconWheelEventManager::WinTrayIconWheelEventManager() {
    m_hookThread.reset(new QThread());
    m_hookThread->setObjectName(lit("GlobalMouseHookThread"));

    connect(m_hookThread.get(), &QThread::started, [&] {
        WinGlobalMouseHook* hook = new WinGlobalMouseHook();

        connect(m_hookThread.get(), &QThread::finished, hook, &QObject::deleteLater);
        connect(hook, &WinGlobalMouseHook::messageHooked, this, &WinTrayIconWheelEventManager::processMessage);
    });

    /* We want the hook proc to always return FAST, thus the custom priority here. */
    m_hookThread->start(QThread::HighestPriority);
}

WinTrayIconWheelEventManager::~WinTrayIconWheelEventManager() {
    m_hookThread->exit();
    m_hookThread->wait();
}

void WinTrayIconWheelEventManager::registerTrayIcon(QSystemTrayIcon* icon) {
    assert(!m_icons.contains(icon));

    m_icons.insert(icon);
}

void WinTrayIconWheelEventManager::unregisterTrayIcon(QSystemTrayIcon* icon) {
    assert(m_icons.contains(icon));

    m_icons.erase(icon);
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
            int delta = -GET_WHEEL_DELTA_WPARAM(data.mouseData);

            QWheelEvent event(
                globalPoint - globalGeometry.topLeft(),
                globalPoint,
                QPoint(),
                message == WM_MOUSEWHEEL ? QPoint(0, delta) : QPoint(delta, 0),
                Qt::NoButton,
                Qt::NoModifier,
                Qt::NoScrollPhase,
                /*inverted=*/ false,
                Qt::MouseEventSynthesizedByApplication);

            qApp->notify(icon, &event);
        }
    }
}
