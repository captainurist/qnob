#include <gtest/gtest.h>

#include "format.h"

TEST(Format, TestFormat) {
    EXPECT_EQ(format(lit("{} {}"), 42, lit("22")), lit("42 22"));
    EXPECT_EQ(format(lit("{0} {1}"), 42, lit("22")), lit("42 22"));
    EXPECT_EQ(format(lit("{1} {0}"), 42, lit("22")), lit("22 42"));
}
