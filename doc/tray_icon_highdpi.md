The Problem
===========

We want to update tray icon whenever screen dpi changes so that it's always crisp.


Solution
========

All is fucked in highdpi world on windows.

Qt has highdpi support, and it might even work for tray icons, but you'll need to supply a custom `QIconEngine`. 
This doesn't work for me because I had to disable Qt highdpi support as it tends to blow up in weird ways when 
changing monitor dpi (or plugging in new monitors with different dpis).

So what we're left is:
1. Detect when DPI change has occured.
2. Get new tray icon size.
3. Update tray icon accordingly.

How to detect DPI changes? It's possible to catch `WM_DISPLAYCHANGE`, then use `FindWindow` to find system tray 
(`Shell_TrayWnd` class), then use `GetDpiForWindow`. Only it doesn't work. System tray dpi hasn't been updated yet 
when you receive `WM_DISPLAYCHANGE`.

OK, another option is to use `GetDpiForMonitor` instead. This returns the new dpi, and you can get the primary monitor
with a call to `MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY)`. This returns the correct updated dpi, but still 
doesn't solve our problem. System tray dpi hasn't been updated yet (see above), so it downscales the supplied icon, 
then explorer handles the dpi change, and then... well. You can guess I was getting pretty frustrated by this point.

So I was thinking that MS might have given this problem some thought and it might make sense to put all the logic into
`WM_DPICHANGED` handler for the message window associated with some tray icon. Nah, same results. Which makes sense, 
if you think about it.

Then I discovered that system tray actually gets recreated every time dpi is changed, and thus all top-level windows
receive `TaskbarCreated` custom message. What is interesting is that `QSystemTrayIcon` actually handles this message, 
and updates the tray icon (calls `Shell_NotifyIcon`), so everything should just work, right? Nah, apparently explorer 
caches the `HICON`, and doesn't even bother to update the actual image if you call `Shell_NotifyIcon` with the same 
`HICON` twice (and this is exactly what `QSystemTrayIcon` does). I didn't bother to check it in x64dbg, but I'd bet 
this is exactly what's happening. This, btw, is a bug in explorer.

However, this also provides a solution - create a new HICON inside the `TaskbarCreated` handler, pass it into 
`Shell_NotifyIcon`, and you're set. Or just call `QSystemTrayIcon::setIcon` with a new `QIcon`.
