#include <gtest/gtest.h>

#include <QtCore/QPoint>

#include "format.h"

TEST(Format, TestSFormat) {
    EXPECT_EQ(sformat(lit("{} {}"), 42, lit("22")), lit("42 22"));
    EXPECT_EQ(sformat(lit("{0} {1}"), 42, lit("22")), lit("42 22"));
    EXPECT_EQ(sformat(lit("{1} {0}"), 42, lit("22")), lit("22 42"));
    EXPECT_EQ(sformat(L"{} {}", 42, lit("22")), lit("42 22"));
}

TEST(Format, TestSFormatDebug) {
    EXPECT_EQ(sformat(L"{}", QPoint(10, 99)), lit("QPoint(10,99)"));
}

TEST(Format, TestSFormatQLatin1String) {
    EXPECT_EQ(sformat(L"{}", QLatin1String("123")), lit("123"));
}

TEST(Format, TestSFormatQUtf8StringView) {
    EXPECT_EQ(sformat(L"{}", QUtf8StringView("123")), lit("123"));
    EXPECT_EQ(sformat(L"{}", QUtf8StringView("\xd0\x81")), QString::fromUcs4(U"\x00000401"));
    EXPECT_EQ(sformat(L"{}", QUtf8StringView("\xF0\x9D\x84\x9E")), QString::fromUcs4(U"\x0001D11E"));
}

TEST(Format, TestSFormatQAnyStringView) {
    EXPECT_EQ(sformat(L"{}", QAnyStringView(QLatin1String("123"))), lit("123"));
    EXPECT_EQ(sformat(L"{}", QAnyStringView(QUtf8StringView("\xd0\x81"))), QString::fromUcs4(U"\x00000401"));
    EXPECT_EQ(sformat(L"{}", QAnyStringView(QString::fromUcs4(U"\x0001D11E"))), QString::fromUcs4(U"\x0001D11E"));
}

TEST(Format, TestSFormatQByteArray) {
    // sformat("{}", QByteArray("1")); /* Should not compile! */
}

TEST(Format, TestBFormat) {
    EXPECT_EQ(bformat(QByteArrayLiteral("123 = {}"), 123), QByteArrayLiteral("123 = 123"));
}

TEST(Format, TestBFormatDebug) {
    EXPECT_EQ(bformat("{0} {0}", QPoint(10, 99)), QByteArrayLiteral("QPoint(10,99) QPoint(10,99)"));
}

TEST(Format, TestBFormatQString) {
    // bformat("{}", lit("1")); /* Should not compile! */
}
