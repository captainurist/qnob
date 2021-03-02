0.1:
* Skinnable tray icon entity.
* Mouse wheel events for tray icons on Win.
* Mouse wheel events for built-in tray icons (yeah, I'm talking about volume).
* Update tooltip after a wheel event on a tray icon.
* Shafts directly exposed in the entity model.
* Knob API: rotateUp, rotateDown, toggle.
* Duck typing in factories (through qt metacalls).
* Lua script entity, with access to globals.
* Generic knob with acceleration (like in win10) and finer details at 0.
* Lua knob for monitor brightness that also changes contrast.
* Utility shafts for (all) external monitors & built-in monitor.
* Multi-shaft OSD for the lua knob.
* --list option to list built-in entities. Or rather a mode, `qnob list`.
* Similar option to list all monitors. `qnob dump`.
* Description for built-in entities.

0.2:
* GUI

1.0:
* AHK -> lua translator
