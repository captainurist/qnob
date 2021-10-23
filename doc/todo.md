0.1:
* **DONE**. Skinnable tray icon entity.
* **DONE**. Numeric skin for tray icons.
* **DONE**. Settings directly exposed in the entity model.
* **DONE**. Knob API: increase, decrease, toggle.
* **DONE**. Mouse wheel events for tray icons on Win. 
* **DONE**. Mouse wheel events for built-in tray icons (yeah, I'm talking about volume).
* **DONE**. Update tooltip after a wheel event on a tray icon.
* **DONE**. Build via MSVC cmake project.
* **DONE**. Basic command line options (--config at least).
* **DONE**. Option to log to console always (even in release).
* **DONE**. Proper logging in release mode (into a log file).
* **DONE**. Fix icons & OSD looking like shit on hidpi displays.
* **DONE**. --list option to list built-in entities. Or rather `--list entities`.
* **DONE**. Some option to list bindable keys, `--list keys`?
* Make qnob back into a console app. Add an option to hide console at startup. Whoever cares about console blinking can start qnob with CreateProcess(CREATE_NO_WINDOW).
* Mouse wheel over tray icon events don't work with a touchpad.
* Monitor controls don't work with a laptop monitor.
* HEOS controls. Input, volume.
* Duck typing in factories (through qt metacalls).
* Lua script entity, with access to globals.
* Generic knob with acceleration (like in win10) and finer details at 0.
* Lua knob for monitor brightness that also changes contrast.
* Utility shafts for (all) external monitors & built-in monitor.
* Multi-shaft OSD for the lua knob.
* Similar option to list all monitors. `--list monitors`?
* Description for built-in entities. `--list verbose-entities`?

0.2:
* GUI
* Philips Hue controls. Colorloop, mode, brightness.

1.0:
* AHK -> lua translator? Looks like an overkill.
