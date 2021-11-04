#include <gtest/gtest.h>

#include "debug.h"

QtMsgType lastMessageType = QtFatalMsg;
QString lastMessageText;
size_t totalMessages = 0;

static void testingMessageHandler(QtMsgType type, const QMessageLogContext&, const QString& msg) {
    lastMessageType = type;
    lastMessageText = msg;
    totalMessages++;
}

TEST(Debug, TestInvalidFormatString) {
    qInstallMessageHandler(&testingMessageHandler);
    auto guard = QScopeGuard([] { qInstallMessageHandler(nullptr); });

    xWarning("{1}", 11);

    EXPECT_EQ(totalMessages, 1);
    EXPECT_EQ(lastMessageType, QtWarningMsg);
    EXPECT_TRUE(lastMessageText.contains(lit("Could not format pattern \"{1}\":")));
}
