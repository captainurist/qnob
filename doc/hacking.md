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

* Use `std::unique_ptr` instead of `QScopedPointer`.
* Declare constructors for `QObject`-derived classes that take `QObject* parent` as the last argument. Do not default parent to `nullptr`!
* Always pass said parent (exceptions are top-level windows and `Entity` classes).
* Use `QObject` parent-child system for `QObject` ownership. It makes `moveToThread` work.
* Also store owned `QObject`s in `std::unique_ptr` fields. This gives fine control over destruction order.
* If a function allocates, it should return `std::unique_ptr`.
* If a function takes ownership of a passed object, it should take `std::unique_ptr`.


Open Questions
==============

* What are our policies regarding exceptions? How do they interoperate with UI and with i18n? Search for `#exceptions` in code.
