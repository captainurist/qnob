#pragma once

#include <array>

#include "logger.h"

class QFile;

class FileLogger : public Logger {
public:
    FileLogger(QFile* file);

    virtual void log(QtMsgType type, const QMessageLogContext& context, const QString& message) override;

private:
    std::array<const char*, 5> m_prefixes;
    QFile* m_file;
};
