#pragma once

#include "file_logger.h"

#include <QtCore/QFile>
#include <QtCore/QStringBuilder>

#include <util/source_root.h>

FileLogger::FileLogger(QFile* file) :
    m_file(file)
{
    m_prefixes[QtDebugMsg] = "DEBG";
    m_prefixes[QtInfoMsg] = "INFO";
    m_prefixes[QtWarningMsg] = "WARN";
    m_prefixes[QtCriticalMsg] = "CRIT";
    m_prefixes[QtFatalMsg] = "FATL";
}

void FileLogger::log(QtMsgType type, const QMessageLogContext& context, const QString& message) {
    QByteArray extendedMessage =
        m_prefixes[type] %
        QByteArrayLiteral("| ") %
        rootRelativeSourcePath(context.file).data() %
        QByteArrayLiteral(":") %
        QByteArray::number(context.line) %
        QByteArrayLiteral(": ");

    qsizetype pos = 0;
    while (pos < message.size()) {
        qsizetype nextPos = message.indexOf(L'\n', pos);
        if (nextPos == -1)
            nextPos = message.size();

        if (pos != 0)
            extendedMessage += QByteArrayLiteral("    | ");

        extendedMessage += QStringView(message.data() + pos, message.data() + nextPos).toUtf8();
        extendedMessage += QByteArrayLiteral("\n");
        pos = nextPos + 1;
    }

    QMutexLocker locker(&m_mutex);
    m_file->write(extendedMessage);
    m_file->flush();
}
