0.1:
* **DONE**. Skinnable tray icon entity.
* **DONE**. Numeric skin for tray icons.
* **DONE**. Settings directly exposed in the entity model.
* **DONE**. Knob API: increase, decrease, toggle.
* **DONE**. Mouse wheel events for tray icons on Win. 
* **DONE**. Mouse wheel events for built-in tray icons (yeah, I'm talking about volume).
* Support [key names from Quake](https://github.com/id-Software/Quake/blob/master/WinQuake/keys.c).
* Update tooltip after a wheel event on a tray icon.
* Duck typing in factories (through qt metacalls).
* Lua script entity, with access to globals.
* Generic knob with acceleration (like in win10) and finer details at 0.
* Lua knob for monitor brightness that also changes contrast.
* Utility shafts for (all) external monitors & built-in monitor.
* Multi-shaft OSD for the lua knob.
* --list option to list built-in entities. Or rather `--list entities`.
* Some option to list bindable keys, `--list keys`?
* Similar option to list all monitors. `--list monitors`?
* Description for built-in entities. `--list verbose-entities`?
* HEOS controls. Input, volume.

0.2:
* GUI
* Philips Hue controls. Colorloop, mode, brightness.

1.0:
* AHK -> lua translator? Looks like an overkill.
