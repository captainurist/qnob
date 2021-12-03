# Intellisense

To make IntelliSense work with custom-built Qt:
1. Create an empty C++ project.
2. Open View > Property Manager.
3. Open `Microsoft.Cpp.x64.user`.
4. Add Qt include dir to Common Properties > VC++ Directories > General > Include Directories.
5. Do the same for `Microsoft.Cpp.Win32.user`.
6. Restart Visual Studio, IntelliSense should be able to find Qt now.


# Code Style
## Using QObject
* Declare constructors for `QObject`-derived classes that take `QObject* parent` as the last argument. Do not default parent to `nullptr`!
* Always pass said parent explicitly.
* Use `QObject` parent-child system for `QObject` ownership. It makes `moveToThread` work.

## Using unique_ptr
* Use `std::unique_ptr` instead of `QScopedPointer`.
* Use `std::make_unique` for allocations. There should be no need to call `new`.
* Store owned `QObject`s in `std::unique_ptr` fields. This gives fine control over destruction order.
* If a function allocates, it should return `std::unique_ptr`.
* If a function takes ownership of a passed object, it should take `std::unique_ptr`, and call `QObject::setParent`.

## Event loops & event processing
* Don't start nested event loops.
* Don't do event processing in destructors. Signals are disconnected in `QObject` destructor, so starting an event loop might lead to calls into partially destroyed objects.

Note that these points provide some very important guarantees: 
* Destructors always finish in a deterministic way.
* Destructor execution will never be non-deterministically interlaced with code that uses a partially destroyed object. It might happen in a deterministic way, but then it's a bug that should be easy to catch.

## Naming
* Use `-Internal` suffix for implementation detail functions that user-facing functions delegate to. E.g. `load` vs `loadInternal`.
* Use `-Locked` suffic for implementation detail functions that do the same thing as the corresponding user-facing function, but expect to be called under lock. Make sure to add `assert(!mutex.try_lock())` to check that.

## Open Questions
* What are our policies regarding exceptions? How do they interoperate with UI and with i18n? Search for `#exceptions` in code.

