#include "message_logger.h"

#include <QtCore/QString>

MessageLogger::MessageLogger() {
    m_prefixes[QtDebugMsg] = "DEBG";
    m_prefixes[QtInfoMsg] = "INFO";
    m_prefixes[QtWarningMsg] = "WARN";
    m_prefixes[QtCriticalMsg] = "CRIT";
    m_prefixes[QtFatalMsg] = "FATL";
}

void MessageLogger::log(QtMsgType type, const QMessageLogContext& context, const QString& message) {
    flushPendingMessages();

    // TODO: cut path!
    QByteArray extendedMessage = lit("%1: %2 (%3:%4)")
        .arg(QLatin1String(m_prefixes[type]))
        .arg(message)
        .arg(QLatin1String(context.file))
        .arg(context.line).toUtf8();

    int written = printMessage(extendedMessage);
    if (written < 0)
        m_pendingMessages.push_back(extendedMessage); // TODO: not thread-safe.
}

void MessageLogger::flushPendingMessages() {
    if (m_pendingMessages.empty())
        return;

    int written = printMessage(m_pendingMessages[0]);
    if (written < 0)
        return;

    for (size_t i = 1; i < m_pendingMessages.size(); i++)
        printMessage(m_pendingMessages[i]);

    m_pendingMessages.clear();
}

int MessageLogger::printMessage(const QByteArray& message) {
    return fprintf(stderr, "%s\n", message.constData());
}
