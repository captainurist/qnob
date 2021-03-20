The Problem
===========

What we want is for the tray icon tooltip to update automatically when we change audio volume.


Solution
========

Tool tip window has `tooltips_class32` window class. So we can find the only tooltip shown on screen via `FindWindowExW` and then checking for `IsWindowVisible`.

This will get us a tool window that has a wndproc similar to how it's defined in [reactos sources](https://doxygen.reactos.org/d4/d96/dll_2win32_2comctl32_2tooltips_8c.html#a54b8079575b0739e3be4ec9c841c6a14) (`TOOLTIPS_WindowProc` function, in case the link is broken).

What we have there:
* `WM_GETTEXT`. Windows does all the marshaling for us, so this just works, and returns the tooltip text. Which means that we're on the right track.
* `TTM_GETTOOLCOUNT`. This one returns the number of tools inside a tooltip, and I have over 10 of these. Looks like it's one per tray icon.
* `TTM_GETCURRENTTOOLW` retrieves extended info about the tool, but it's a `WM_USER` message, and it doesn't get marshaled. So we need to roll something on our own, and this is what I came up with:

  ```cpp
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);

    TTTOOLINFOW localInfo;
    memset(&localInfo, 0, sizeof(localInfo));
    localInfo.cbSize = sizeof(localInfo);

    LPVOID remoteInfo = VirtualAllocEx(process, NULL, sizeof(TTTOOLINFOW), MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(process, remoteInfo, &localInfo, sizeof(localInfo), NULL);

    SendMessageW(hwnd, TTM_GETCURRENTTOOLW, 0, reinterpret_cast<LPARAM>(remoteInfo));

    ReadProcessMemory(process, remoteInfo, &localInfo, sizeof(localInfo), NULL);

    qDebug() 
        << "tool flags =" << localInfo.uFlags << "hwnd =" << localInfo.hwnd 
        << "id =" << localInfo.uId << "text = " << localInfo.lpszText;

    VirtualFreeEx(process, remoteInfo, NULL, MEM_RELEASE);
    CloseHandle(process);
  ```

  And surprise, `lpszText` is actually `LPSTR_TEXTCALLBACKW`. This makes our life x100 easier, because:
* `TTM_UPDATE` has no parameters (and thus doesn't need dark magic above for marshaling), updates tooltip text, and redraws it. This is what we need!

So what is needed then is to just post a `TTM_UPDATE` and that's it!
