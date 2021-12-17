#include "win_functions.h"

#include <Windows.h>
#include <CommCtrl.h>

#include <util/debug.h>

#include "win_error.h"

static INPUT makeKeyInput(WORD key, DWORD flags = 0) {
    INPUT result;
    memset(&result, 0, sizeof(result));
    result.type = INPUT_KEYBOARD;
    result.ki.dwFlags = flags;

    if (flags & KEYEVENTF_UNICODE) {
        result.ki.wScan = key;
    } else {
        result.ki.wVk = key;
    }

    return result;
}

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
    if (text.isEmpty())
        return;

    QVarLengthArray<INPUT, 32> buffer;
    for (QChar c: text) {
        buffer.push_back(makeKeyInput(c.unicode(), KEYEVENTF_UNICODE));
        buffer.push_back(makeKeyInput(c.unicode(), KEYEVENTF_UNICODE | KEYEVENTF_KEYUP));
    }

    apicall(
        SendInput(buffer.size(), buffer.data(), sizeof(INPUT)),
        [&](auto count) { return count == buffer.size(); }
    );
}

void WinFunctions::winUpdateCurrentToolTip() const {
    HWND hwnd = NULL;

    while (hwnd = FindWindowExW(NULL, hwnd, L"tooltips_class32", NULL)) {
        if (!IsWindowVisible(hwnd))
            continue;

        apicall(PostMessageW(hwnd, TTM_UPDATE, 0, 0));
        return;
    }
}

void WinFunctions::winHideConsole() const {
    HWND console = GetConsoleWindow();
    if (console != NULL)
        ShowWindow(console, SW_HIDE);
}

