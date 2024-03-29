# The Problem

What we want is to be able to identify the audio icon in the system tray, so that we could then properly process mouse wheel events for it.

Mouse events are processed in the low-level mouse hook, and icon positions are retrieved via a call to `Shell_NotifyIconGetRect`, which requires either a GUID, or HWND + icon id pair.



# Finding HWND & Icon Id

[Anvir task manager](http://www.anvir.com/download.htm) can be used to identify the process that the tray icon belongs to. Audio icon is created by explorer.exe.

So the steps are as follows:
1. Open spyxx.
2. Open processes view, and open message log for explorer.exe.
3. Set message filter for `WM_USER`.
4. Right click on the audio icon.
5. Examine message log for messages that look like tray icon notifications (see [docs for `NOTIFYICONDATAW`](https://docs.microsoft.com/en-us/windows/win32/api/shellapi/ns-shellapi-notifyicondataw)). The messages will have mouse coordinates in `wParam` and icon id & windows message code in `lParam`. Look for `lParam` that looks like `0x0064007B` (`0x7B` is `WM_CONTEXTMENU`).
6. `HIWORD(lParam)` will contain icon id.
7. Check the window that's processed this message, note its window class & caption.



# Finding out if Explorer uses GUIDs

`Shell_NotifyIconW` is a very simple function, see [reactos sources](https://doxygen.reactos.org/da/d7b/systray_8cpp_source.html). It basically populates an internal structure, and sends it over to the system tray window via `WM_COPYDATA`.

Option #1:
1. Open explorer.exe in x64dbg.
2. Set breakpoint in `Shell_NotifyIconW`.
3. Right click on the audio icon.
4. Examine params.

Option #2:
1. Hook into explorer.exe via `SetWindowsHookExW(WH_GETMESSAGE, ...)`.
2. Look for `WM_COPYDATA`, dump all data.
3. Examine it for GUIDs. Note that this is harder than option #1 above because data layout is not known here (but can be reconstructed by looking at `Shell_NotifyIconW` code).



# Results

For Win 10.0.18363 audio icon id is `0x0064`, and window class is `ATL:00007FFD51668280` (lol), with empty caption. Guid is `73 AE 20 78 E3 23 29 42 82 C1 E4 1C B6 7D 5B 9C`, and this will be easier to use. 

Also somehow this has [already been figured out](https://titanwolf.org/Network/Articles/Article?AID=e112b2ff-5db8-4aee-9de7-fe6be97a743b).

And apparently GUID can be retreived with [UI Automation](https://docs.microsoft.com/en-us/windows/win32/api/uiautomationclient/nf-uiautomationclient-iuiautomation-elementfrompoint).



# Getting Windows Version

Everything's in [this stackoverflow answer](https://stackoverflow.com/questions/47581146/getting-os-build-version-from-win32-api-c). It's either via `GetProductInfo` call, or from `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion` registry key.
