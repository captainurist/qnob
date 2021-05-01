#pragma once

#include "file_logger.h"

#include <QtCore/QFile>
#include <QtCore/QStringBuilder>

#include <util/source_location.h>

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
        QByteArrayLiteral(": ") %
        message.toUtf8() %
        QByteArrayLiteral(" (") %
        (std::string_view(context.file).starts_with(sourceRoot()) ? sourceRootRelativeFilePath(context.file) : context.file) %
        QByteArrayLiteral(":") %
        QByteArray::number(context.line) %
        QByteArrayLiteral(")\n");

    // TODO: lock here? QFile is not thread-safe.

    m_file->write(extendedMessage);
    m_file->flush();
}
