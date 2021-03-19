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
    int delta = -GET_WHEEL_DELTA_WPARAM(data.mouseData);

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

void WinTrayIconWheelEventManager::registerStandardIcon(PlatformStandardTrayIcon standardIcon, QObject* icon) {
    assert(!m_standardIcons.contains(standardIcon));
    m_standardIcons[standardIcon] = icon;
}

void WinTrayIconWheelEventManager::unregisterStandardIcon(PlatformStandardTrayIcon standardIcon) {
    assert(m_standardIcons.contains(standardIcon));
    m_standardIcons.erase(standardIcon);
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
            return; /* Re-registrations are not allowed, so it's safe to return here. */
        }
    }

    for (auto [standardIcon, icon] : m_standardIcons) {
        assert(standardIcon == AudioTrayIcon);

        NOTIFYICONIDENTIFIER identifier;
        identifier.cbSize = sizeof(identifier);
        identifier.hWnd = 0;
        identifier.uID = 0;
        identifier.guidItem = GUID_TrayIconVolume;

        RECT location;
        if (!apicall(Shell_NotifyIconGetRect(&identifier, &location)))
            continue; /* Icon not found? */
        QRect globalGeometry = QRectFromRECT(location);

        if (globalGeometry.contains(globalPoint)) {
            sendSyntheticWheelEvent(icon, globalGeometry, message, data);
            return; /* Re-registrations are not allowed, so it's safe to return here. */
        }
    }
}
