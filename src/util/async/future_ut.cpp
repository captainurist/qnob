#include <gtest/gtest.h>

#include <QtCore/QObject>
#include <QtCore/QEventLoop>
#include <QtCore/QCoreApplication>

#include <lib/test/test_application.h>

#include "future.h"
#include "promise.h"

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

TEST(Future, TestVoid) {
    Promise<void> promise;
    Future<void> future = promise.future();
    promise.set_value();
    EXPECT_NO_THROW(future.get());
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

TEST(Future, TestQObjectIntChain) {
    Promise<int> promise;

    QObject o1, o2, o3;
    TestApplication<QCoreApplication> app;

    auto inc = [](int value) { return value + 1; };

    Future<int> chain = promise.future().then(&o1, inc).then(&o2, inc).then(&o3, inc).then(&app, [&](int value) { app.exit(); return value; });
    promise.set_value(0);

    app.exec();

    EXPECT_EQ(chain.get(), 3);
}

TEST(Future, TestQObjectVoidChain) {
    Promise<void> promise;

    QObject o1, o2, o3;
    TestApplication<QCoreApplication> app;

    size_t counter = 0;
    auto inc = [&] { counter++; };

    Future<void> chain = promise.future().then(&o1, inc).then(&o2, inc).then(&o3, inc).then(&app, [&] { app.exit(); });
    promise.set_value();

    app.exec();

    EXPECT_EQ(counter, 3);
    EXPECT_NO_THROW(chain.get());
}

TEST(Future, TestQObjectDestruction) {
    Promise<void> promise;
    FutureResult<void> ret;

    std::unique_ptr<QObject> o1 = std::make_unique<QObject>();
    std::unique_ptr<QObject> o2 = std::make_unique<QObject>();
    std::unique_ptr<QObject> o3 = std::make_unique<QObject>();
    TestApplication<QCoreApplication> app;

    size_t counter = 0;
    auto inc = [&] { counter++; };

    Future<void> chain = promise.future().
        then(o1.get(), inc).
        then(o2.get(), [&] { o3.reset(); }).
        then(o3.get(), inc).
        then(&app, [&](FutureResult<void> result) { ret = std::move(result); app.exit(); });
    promise.set_value();

    app.exec();

    EXPECT_EQ(counter, 1);
    EXPECT_TRUE(ret.has_error());
    EXPECT_FALSE(ret.has_value());
    EXPECT_THROW(ret.get(), FutureContextException);
}

// TODO: cancellation token tests
