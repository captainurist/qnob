#include <gtest/gtest.h>

#include "exception.h"

TEST(Exception, TestAutoChain) {
    try {
        try {
            xthrow Exception(QLatin1String("Exception 1"));
        } catch (...) {
            xthrow Exception(QLatin1String("Exception 2"));
        }
    } catch (const Exception& e) {
        EXPECT_TRUE(e.cause());
    }
}

TEST(Exception, TestDontChain) {
    try {
        try {
            xthrow Exception(QLatin1String("Exception 1"));
        } catch (...) {
            xthrow Exception(QLatin1String("Exception 2"), Exception::DontChain);
        }
    } catch (const Exception& e) {
        EXPECT_FALSE(e.cause());
    }
}
