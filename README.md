Qnob
====
Qnob is a programmable OSD & hotkey tool for Windows 10.


Building
========

Currently only on Windows:
1. Set `CMAKE_PREFIX_PATH` to point to your Qt installation, e.g. `c:\Qt\6.0.0\msvc2019_64`.
2. Add `cmake` and `ninja` to `PATH`.
3. Open qnob as a cmake project in Visual Studio 2019.
4. By default qnob links statically with CRT (`/MT` switch). If your Qt is not statically linked with CRT, then comment out the corresponding line in `CMakeLists.txt` (search for `/MT`).
5. Build.
