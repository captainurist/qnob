#include "win_wheel_event_manager.h"

#include <cassert>
#include <ranges>

#include <QtCore/QThread>
#include <QtCore/QCoreApplication>
#include <QtGui/QWheelEvent>
#include <QtWidgets/QSystemTrayIcon>

#include <platform/platform_control.h>
#include <util/debug.h>

#include "win_shared_event_window.h"
#include "win_guids.h"
#include "win_error.h"

static QRect QRectFromRECT(const RECT& rect) {
    return QRect(QPoint(rect.left, rect.top), QPoint(rect.right, rect.bottom));
}

static void sendSyntheticWheelEvent(QObject* target, const QRect& globalGeometry, const RAWMOUSE& mouseInput, const POINT& cursorPos) {
    int delta = static_cast<SHORT>(mouseInput.usButtonData);

    /* Using GetKeyState here instead of GetAsyncKeyState because this is consistent with how the rest of the message
     * processing works. If a sync function returns true this basically means that the corresponding key press message
     * was delivered, and this is what we need here. */
    Qt::KeyboardModifiers mods;
    if (GetKeyState(VK_SHIFT) < 0)
        mods |= Qt::ShiftModifier;
    if (GetKeyState(VK_CONTROL) < 0)
        mods |= Qt::ControlModifier;
    if (GetKeyState(VK_MENU) < 0)
        mods |= Qt::AltModifier;
    /* No reason to check for VK_LWIN / VK_RWIN here, you can't reasonably use it with mouse wheel over tray icons. */

    QWheelEvent event(
        QPoint(cursorPos.x, cursorPos.y) - globalGeometry.topLeft(),
        QPoint(cursorPos.x, cursorPos.y),
        QPoint(),
        (mouseInput.usButtonFlags & RI_MOUSE_WHEEL) ? QPoint(0, delta) : QPoint(delta, 0),
        Qt::NoButton,
        Qt::NoModifier,
        Qt::NoScrollPhase,
        /*inverted=*/ false,
        Qt::MouseEventSynthesizedByApplication);
    event.setModifiers(mods);

    qApp->notify(target, &event);
}

WinWheelEventManager::WinWheelEventManager(WinSharedEventWindow* eventWindow, QObject* parent):
    PlatformWheelEventManager(parent)
{
    if (qgetenv("QT_ENABLE_HIGHDPI_SCALING") != "0")
        xCritical("QT_ENABLE_HIGHDPI_SCALING != 0, trayicon mouse wheel events might not work on highdpi displays.");

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
        xWarning("GetRawInputData failed");
        return;
    }

    if (input.header.dwType != RIM_TYPEMOUSE)
        return; /* Should never happen. */

    const RAWMOUSE& mouseInput = input.data.mouse;
    if (!(mouseInput.usButtonFlags & (RI_MOUSE_WHEEL | RI_MOUSE_HWHEEL)))
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
