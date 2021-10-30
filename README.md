Qnob
====
Qnob is a programmable OSD & hotkey tool for Windows 10.


Building
========

Currently only on Windows:
1. Get MSVC 2019 (16.11 or later).
2. Get a fresh Windows SDK (10.0.22000 or later).
3. Get Qt 6.2.
4. Apply the patches in `3rdparty/qt`.
5. Build Qt. If you want a static build, there are instructions in [doc/qt.md](doc/qt.md).
6. Set `CMAKE_PREFIX_PATH` to point to your Qt build, e.g. `c:\Qt\6.0.0\msvc2019_64`.
7. Add `cmake` and `ninja` to `PATH`.
8. Open qnob as a cmake project in Visual Studio 2019.
9. By default qnob links statically with CRT (`/MT` switch). If your Qt is not statically linked with CRT, then comment out the corresponding line in `CMakeLists.txt` (search for `MSVC_RUNTIME_LIBRARY`).
10. Build.

