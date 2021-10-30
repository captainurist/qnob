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
    // TODO: prefix before each \n
    QByteArray extendedMessage =
        m_prefixes[type] %
        QByteArrayLiteral(": ") %
        message.toUtf8() %
        QByteArrayLiteral(" (") %
        relativeSourcePath(context.file).data() %
        QByteArrayLiteral(":") %
        QByteArray::number(context.line) %
        QByteArrayLiteral(")\n");

    QMutexLocker locker(&m_mutex);
    m_file->write(extendedMessage);
    m_file->flush();
}
