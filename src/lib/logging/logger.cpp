#include "logger.h"

#include <QtCore/QScopeGuard>

static Logger* g_logger = nullptr;

static void loggingMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    g_logger->log(type, context, msg);
}

Logger* Logger::installGlobalLogger(Logger* logger) {
    if (logger == g_logger)
        return g_logger;

    if (!g_logger)
        qInstallMessageHandler(loggingMessageHandler);

    Logger* result = g_logger;
    g_logger = logger;

    if (!g_logger)
        qInstallMessageHandler(nullptr);

    return result;
}
