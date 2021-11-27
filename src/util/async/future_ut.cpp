#include <gtest/gtest.h>

#include <QtCore/QObject>
#include <QtCore/QEventLoop>
#include <QtCore/QCoreApplication>

#include "future.h"
#include "promise.h"

int argc = 1;
char arg0[4] = "arg";
char* argv[2] = { arg0, nullptr };

class GTestCoreApplication : public QCoreApplication {
public:
    GTestCoreApplication() : QCoreApplication(argc, argv) {}
};

struct Counters {
    int copied = 0;
    int moved = 0;
};

class Counting {
public:
    Counting(Counters* counters): 
        m_counters(counters)
    {}

    Counting(const Counting& other) {
        m_counters = other.m_counters;
        m_counters->copied++;
    }

    Counting(Counting&& other) {
        m_counters = other.m_counters;
        m_counters->moved++;
    }

private:
    Counters* m_counters;
};

// TODO: I'd also want to check for assertions, but this is more complex than it might seem.
// On Windows there's _CrtSetReportHook, but this is not portable.

TEST(Future, TestPassValue) {
    Promise<int> promise;
    Future<int> future = promise.future();
    promise.set_value(1);
    EXPECT_EQ(future.get(), 1);
}

TEST(Future, TestPassError) {
    Promise<int> promise;
    Future<int> future = promise.future();
    promise.set_error(std::make_exception_ptr(1));
    EXPECT_THROW(future.get(), int);
}

TEST(Future, TestPassValueDetached) {
    Counters counters;
    Counting counting(&counters);

    {
        Promise<Counting> promise;
        {
            Future<Counting> future = promise.future();
        } /* Let future go out of scope here. */

        promise.set_value(std::move(counting));
    } /* And same for promise. */
    
    EXPECT_EQ(counters.copied, 0);
    EXPECT_EQ(counters.moved, 0);
}

TEST(Future, TestQObjectChain) {
    Promise<int> promise;

    QObject o1, o2, o3;
    GTestCoreApplication app;

    auto inc = [](int value) { return value + 1; };

    Future<int> chain = promise.future().then(&o1, inc).then(&o2, inc).then(&o3, inc).then(&app, [&](int value) { app.exit(); return value; });
    promise.set_value(0);

    app.exec();

    EXPECT_EQ(chain.get(), 3);
}
