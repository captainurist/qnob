#pragma once

#include <QtCore/QMessageLogContext>

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    const char* file = context.file ? context.file : "";
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "DEBG: %s (%s:%u)\n", localMsg.constData(), file, context.line);
        break;
    case QtInfoMsg:
        fprintf(stderr, "INFO: %s (%s:%u)\n", localMsg.constData(), file, context.line);
        break;
    case QtWarningMsg:
        fprintf(stderr, "WARN: %s (%s:%u)\n", localMsg.constData(), file, context.line);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "CRIT: %s (%s:%u)\n", localMsg.constData(), file, context.line);
        break;
    case QtFatalMsg:
        fprintf(stderr, "FATL: %s (%s:%u)\n", localMsg.constData(), file, context.line);
        break;
    }
}
