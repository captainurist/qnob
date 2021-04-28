#include "win_wheel_event_manager.h"

#include <cassert>
#include <ranges>

#include <QtCore/QThread>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtGui/QWheelEvent>
#include <QtWidgets/QSystemTrayIcon>

#include <platform/platform_control.h>

#include "win_shared_event_window.h"
#include "win_guids.h"
#include "win_error.h"

static QRect QRectFromRECT(const RECT& rect) {
    return QRect(QPoint(rect.left, rect.top), QPoint(rect.right, rect.bottom));
}

static void sendSyntheticWheelEvent(QObject* target, const QRect& globalGeometry, const RAWMOUSE& mouseInput, const POINT& cursorPos) {
    int delta = static_cast<SHORT>(mouseInput.usButtonData);

    QWheelEvent event(
        QPoint(cursorPos.x, cursorPos.y) - globalGeometry.topLeft(),
        QPoint(cursorPos.x, cursorPos.y),
        QPoint(),
        mouseInput.usButtonFlags == RI_MOUSE_WHEEL ? QPoint(0, delta) : QPoint(delta, 0),
        Qt::NoButton,
        Qt::NoModifier,
        Qt::NoScrollPhase,
        /*inverted=*/ false,
        Qt::MouseEventSynthesizedByApplication);

    qApp->notify(target, &event);
}

static void checkQtHighDpiDisabled() {
    static bool checked = false;
    if (checked)
        return;
    checked = true;

    if (qgetenv("QT_ENABLE_HIGHDPI_SCALING") != "0")
        qCritical() << "QT_ENABLE_HIGHDPI_SCALING != 0, trayicon mouse wheel events might not work on highdpi displays.";
}

WinWheelEventManager::WinWheelEventManager(WinSharedEventWindow* eventWindow) {
    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x0001;
    rid.usUsage = 0x0002; /* Generic mouse. */
    rid.dwFlags = RIDEV_INPUTSINK;
    rid.hwndTarget = reinterpret_cast<HWND>(eventWindow->winId());
    apicall(RegisterRawInputDevices(&rid, 1, sizeof(rid)));

    connect(eventWindow, &WinSharedEventWindow::input, this, &WinWheelEventManager::processInput);
}

WinWheelEventManager::~WinWheelEventManager() {}

void WinWheelEventManager::registerTrayIcon(QSystemTrayIcon* icon) {
    assert(!m_icons.contains(icon));

    /* Highdpi implementation in Qt is a mess, and making it work proved to be harder than I thought.
     * So we just check that it's disabled. */
    checkQtHighDpiDisabled();

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

void WinWheelEventManager::processInput(MSG* message) {
    assert(message->message == WM_INPUT);

    HRAWINPUT inputHandle = reinterpret_cast<HRAWINPUT>(message->lParam);
    RAWINPUT input;
    UINT inputSize = sizeof(input);
    UINT bytesCopied = GetRawInputData(inputHandle, RID_INPUT, &input, &inputSize, sizeof(RAWINPUTHEADER));
    if (bytesCopied < 0) {
        qWarning() << "GetRawInputData failed.";
        return;
    }

    if (input.header.dwType != RIM_TYPEMOUSE)
        return; /* Should never happen. */

    const RAWMOUSE& mouseInput = input.data.mouse;
    if (mouseInput.usButtonFlags != RI_MOUSE_WHEEL && mouseInput.usButtonFlags != RI_MOUSE_HWHEEL)
        return; /* We're not interested in clicks & mouse move events. */

    if (mouseInput.usButtonData == 0)
        return; /* Should never happen. */

    POINT cursorPos;
    if (!apicall(GetCursorPos(&cursorPos)))
        return;

    HWND window = WindowFromPoint(cursorPos);
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

    QPoint globalPoint(cursorPos.x, cursorPos.y);
    for (QSystemTrayIcon* icon : m_icons) {
        /* Note: if Qt highdpi is enabled, the call below returns geometry in device-independent pixels.
         * This obviously doesn't work with cursor position returned by the OS. */
        QRect globalGeometry = icon->geometry();

        if (globalGeometry.contains(globalPoint)) {
            sendSyntheticWheelEvent(icon, globalGeometry, mouseInput, cursorPos);
            return;
        }
    }

    for (PlatformControl* control : m_controls) {
        QRect globalGeometry = control->geometry();

        if (globalGeometry.contains(globalPoint)) {
            sendSyntheticWheelEvent(control, globalGeometry, mouseInput, cursorPos);
            /* Can't return here, can have duplicates in standard controls. */
        }
    }
}
