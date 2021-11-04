#include <gtest/gtest.h>

#include <QtCore/QPoint>

#include "format.h"

TEST(Format, TestSFormat) {
    EXPECT_EQ(sformat(lit("{} {}"), 42, lit("22")), lit("42 22"));
    EXPECT_EQ(sformat(lit("{0} {1}"), 42, lit("22")), lit("42 22"));
    EXPECT_EQ(sformat(lit("{1} {0}"), 42, lit("22")), lit("22 42"));

    EXPECT_EQ(sformat(L"{} {}", 42, lit("22")), lit("42 22"));
}

TEST(Format, TestQDebugSFormat) {
    EXPECT_EQ(sformat(L"{}", QPoint(10, 99)), lit("QPoint(10,99)"));
}

TEST(Format, TestBFormat) {
    EXPECT_EQ(bformat(QByteArrayLiteral("123 = {}"), 123), QByteArrayLiteral("123 = 123"));
}

TEST(Format, TestQDebugBFormat) {
    EXPECT_EQ(bformat("{0} {0}", QPoint(10, 99)), QByteArrayLiteral("QPoint(10,99) QPoint(10,99)"));
}
