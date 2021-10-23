Qnob
====
Qnob is a programmable OSD & hotkey tool for Windows 10.


Building
========

Currently only on Windows:
1. Get MSVC 2019 (16.11 or later).
2. Get Qt 6.2.
3. Apply the patches in `3rdparty/qt`.
4. Build Qt. If you want a static build, there are instructions in [doc/qt.md](doc/qt.md).
5. Set `CMAKE_PREFIX_PATH` to point to your Qt build, e.g. `c:\Qt\6.0.0\msvc2019_64`.
6. Add `cmake` and `ninja` to `PATH`.
7. Open qnob as a cmake project in Visual Studio 2019.
8. By default qnob links statically with CRT (`/MT` switch). If your Qt is not statically linked with CRT, then comment out the corresponding line in `CMakeLists.txt` (search for `MSVC_RUNTIME_LIBRARY`).
9. Build.

Hacking
=======

To make IntelliSense work with custom-built Qt:
1. Create an empty C++ project.
2. Open View > Property Manager.
3. Open `Microsoft.Cpp.x64.user`.
4. Add Qt include dir to Common Properties > VC++ Directories > General > Include Directories.
5. Do the same for `Microsoft.Cpp.Win32.user`.
6. Restart Visual Studio, IntelliSense should be able to find Qt now.
