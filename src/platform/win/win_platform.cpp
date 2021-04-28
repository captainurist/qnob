#include "win_platform.h"

#include <Windows.h>
#include <CommCtrl.h>
#include <io.h>
#include <fcntl.h>

#include <cassert>

#include <QtCore/QThread>
#include <QtCore/QVariant>

#include "com.h"
#include "win_crtio.h"
#include "win_error.h"
#include "win_volume_control.h"
#include "win_monitor_manager.h"
#include "win_shortcut_manager.h"
#include "win_wheel_event_manager.h"
#include "win_metrics.h"
#include "win_shared_event_window.h"
#include "win_standard_control.h"

WinPlatform::WinPlatform() {
    m_com.reset(new Com());
    m_eventWindow.reset(new WinSharedEventWindow());
    m_volumeControl.reset(new WinVolumeControl());
    m_monitorManager.reset(new WinMonitorManager(m_eventWindow.get()));
    m_shortcutManager.reset(new WinShortcutManager(m_eventWindow.get()));
    m_wheelEventManager.reset(new WinWheelEventManager(m_eventWindow.get()));
    m_metrics.reset(new WinMetrics(m_eventWindow.get()));
}

WinPlatform::~WinPlatform() {}

PlatformVolumeControl* WinPlatform::volumeControl() const {
    return m_volumeControl.get();
}

PlatformMonitorManager* WinPlatform::monitorManager() const {
    return m_monitorManager.get();
}

PlatformShortcutManager* WinPlatform::shortcutManager() const {
    return m_shortcutManager.get();
}

PlatformWheelEventManager* WinPlatform::wheelEventManager() const {
    return m_wheelEventManager.get();
}

PlatformMetrics* WinPlatform::metrics() const {
    return m_metrics.get();
}

PlatformControl* WinPlatform::createStandardControl(PlatformStandardControl control) const {
    return new WinStandardControl(control);
}

QVariant WinPlatform::execute(PlatformFunction function, QVariant arg0) {
    switch (function) {
    case GetConsoleSize:
        if (QSize size = getConsoleSize(); size.isValid())
            return size;
        return QVariant();
    case WinUpdateCurrentToolTip:
        updateCurrentToolTip();
        return QVariant();
    case WinEnsureConsole:
        ensureConsole();
        return QVariant();
    case WinIsConsoleOwned:
        return m_ownsConsole;
    default:
        assert(false);
        return QVariant();
    }
}

void WinPlatform::updateCurrentToolTip() const {
    HWND hwnd = NULL;

    while (hwnd = FindWindowExW(NULL, hwnd, L"tooltips_class32", NULL)) {
        if (!IsWindowVisible(hwnd))
            continue;

        PostMessageW(hwnd, TTM_UPDATE, 0, 0);
        return;
    }
}

QSize WinPlatform::getConsoleSize() const {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (apicall(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))) {
        return QSize(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
    } else {
        return QSize();
    }
}

void WinPlatform::ensureConsole() {
    if (GetConsoleWindow() != NULL)
        return; /* Already have a console. */

    if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
        /* Parent has no console, allocate a new one. */
        if (!apicall(AllocConsole()))
            return; /* Failed. Not much we can do at this point. */
        m_ownsConsole = true;
    }

    /* Now redirect everything. */
    int stdinFd = _open_osfhandle(reinterpret_cast<intptr_t>(GetStdHandle(STD_INPUT_HANDLE)), _O_RDONLY);
    int stdoutFd = _open_osfhandle(reinterpret_cast<intptr_t>(GetStdHandle(STD_OUTPUT_HANDLE)), _O_WRONLY | _O_TEXT);
    int stderrFd = _open_osfhandle(reinterpret_cast<intptr_t>(GetStdHandle(STD_ERROR_HANDLE)), _O_WRONLY | _O_TEXT);
    bool stdinOk = _dup2(stdinFd, 0) == 0;
    bool stdoutOk = _dup2(stdoutFd, 1) == 0;
    bool stderrOk = _dup2(stderrFd, 2) == 0;
    if (!stdinOk || !stdoutOk || !stderrOk)
        qWarning() << "Could not redirect some of the standard streams";

    /* Make stdin/stdout/stderr work. Meddling with CRT internals is OK as we're linking statically anyway. */
    crtData(stdin)->_file = 0;
    crtData(stdout)->_file = 1;
    crtData(stderr)->_file = 2;

    /* Note that there is no need to close the old fds. It's not like we'll be running this code twice. */
}

Platform* createPlatform() {
    return new WinPlatform();
}
