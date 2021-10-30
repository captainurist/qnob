Intellisense
=======

To make IntelliSense work with custom-built Qt:
1. Create an empty C++ project.
2. Open View > Property Manager.
3. Open `Microsoft.Cpp.x64.user`.
4. Add Qt include dir to Common Properties > VC++ Directories > General > Include Directories.
5. Do the same for `Microsoft.Cpp.Win32.user`.
6. Restart Visual Studio, IntelliSense should be able to find Qt now.


Code Style
==========

* Use std::unique_ptr instead of QScopedPointer.
* Use std::unique_ptr instead of QObject parent-child system where possible.
* ...

