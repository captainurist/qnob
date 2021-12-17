#include "win_functions.h"

#include <Windows.h>
#include <CommCtrl.h>

#include "win_error.h"

WinFunctions::WinFunctions(QObject* parent) :
    PlatformFunctions(parent)
{}

QSize WinFunctions::consoleSize() const {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (apicall(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))) {
        return QSize(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
    } else {
        return QSize();
    }
}

void WinFunctions::sendInput(const QString& text) const {
    // TODO
}

void WinFunctions::winUpdateCurrentToolTip() const {
    HWND hwnd = NULL;

    while (hwnd = FindWindowExW(NULL, hwnd, L"tooltips_class32", NULL)) {
        if (!IsWindowVisible(hwnd))
            continue;

        PostMessageW(hwnd, TTM_UPDATE, 0, 0);
        return;
    }
}

void WinFunctions::winHideConsole() const {
    HWND console = GetConsoleWindow();
    if (console != NULL)
        ShowWindow(console, SW_HIDE);
}

