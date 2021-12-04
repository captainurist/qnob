#pragma once

namespace detail {
    inline char g_testPath[] = __FILE__;
    inline char* g_testArgv[] = { g_testPath, nullptr };
    inline int g_testArgc = 1;
}

/**
 * `QCoreApplication` / `QApplication` wrapper to be used in unit tests.
 *
 * \tparam Application                  Actual application type to be used.
 */
template<class Application>
class TestApplication : public Application {
public:
    TestApplication() : Application(detail::g_testArgc, detail::g_testArgv) {}
};
