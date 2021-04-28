#pragma once

#include <array>
#include <vector>

#include <QtCore/QMessageLogContext>
#include <QtCore/QByteArray>

class MessageLogger {
public:
    MessageLogger();

    void log(QtMsgType type, const QMessageLogContext& context, const QString& message);

private:
    void flushPendingMessages();
    int printMessage(const QByteArray& message);

private:
    std::array<const char*, 5> m_prefixes;
    std::vector<QByteArray> m_pendingMessages;
};
