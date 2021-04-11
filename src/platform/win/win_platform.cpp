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
#include "win_global_mouse_hook.h"
#include "win_standard_control.h"

WinPlatform::WinPlatform() {
    m_com.reset(new Com());

    m_hookThread.reset(new QThread());
    m_hookThread->setObjectName(lit("GlobalMouseHookThread"));
    m_hookThread->start(QThread::HighestPriority); /* We want the hook proc to always return FAST, thus the custom priority here. */

    m_hook = new WinGlobalMouseHook();
    m_hook->moveToThread(m_hookThread.get());
    connect(m_hookThread.get(), &QThread::finished, m_hook, &QObject::deleteLater);
    connect(this, &WinPlatform::hookChangeRequested, m_hook, &WinGlobalMouseHook::setEnabled);

    m_volumeControl.reset(new WinVolumeControl());
    m_monitorManager.reset(new WinMonitorManager());
    m_trayIconWheelEventManager.reset(new WinWheelEventManager(m_hook));
    m_shortcutManager.reset(new WinShortcutManager());

    /* Note that hooks are disabled by default, we don't call hookChangeRequested(true) here. */
}

WinPlatform::~WinPlatform() {
    m_hookThread->exit();
    m_hookThread->wait();
}

PlatformVolumeControl* WinPlatform::volumeControl() const {
    return m_volumeControl.get();
}

PlatformMonitorManager* WinPlatform::monitorManager() const {
    return m_monitorManager.get();
}

PlatformShortcutManager* WinPlatform::shortcutManager() const {
    return m_shortcutManager.get();
}

PlatformWheelEventManager* WinPlatform::trayIconWheelEventManager() const {
    return m_trayIconWheelEventManager.get();
}

PlatformControl* WinPlatform::createStandardControl(PlatformStandardControl control) const {
    return new WinStandardControl(control);
}

QVariant WinPlatform::execute(PlatformFunction function) {
    switch (function) {
    case GetConsoleSize:
        if (QSize size = getConsoleSize(); size.isValid())
            return size;
        return QVariant();
    case WinEnableHooks:
        emit hookChangeRequested(true);
        return QVariant();
    case WinDisableHooks:
        emit hookChangeRequested(false);
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
