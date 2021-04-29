#pragma once

#include <QtCore/QMessageLogContext>

class Logger {
public:
    virtual ~Logger() {}

    virtual void log(QtMsgType type, const QMessageLogContext& context, const QString& message) = 0;

    /**
     * Sets the global logger. If non-null logger is passed, also automatically calls `qInstallMessageHandler`.
     *
     * This function does not take ownership of the provided logger.
     *
     * \param logger                    New logger to install as global logger.
     * \returns                         Old global logger, if any.
     */
    static Logger* installGlobalLogger(Logger* logger);
};
