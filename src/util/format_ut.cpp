#include <gtest/gtest.h>

#include <QtCore/QPoint>

#include "format.h"

TEST(Format, TestFormat) {
    EXPECT_EQ(xformat(lit("{} {}"), 42, lit("22")), lit("42 22"));
    EXPECT_EQ(xformat(lit("{0} {1}"), 42, lit("22")), lit("42 22"));
    EXPECT_EQ(xformat(lit("{1} {0}"), 42, lit("22")), lit("22 42"));

    EXPECT_EQ(xformat(L"{} {}", 42, lit("22")), lit("42 22"));
}

TEST(Format, TestQDebugFormat) {
    EXPECT_EQ(xformat(L"{}", QPoint(10, 99)), lit("QPoint(10,99)"));
}
