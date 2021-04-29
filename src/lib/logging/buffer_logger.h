#pragma once

#include <deque>

#include "logger.h"

class BufferLogger : public Logger {
public:
    BufferLogger(size_t maxSize = 1024) :
        m_maxSize(maxSize)
    {}

    virtual void log(QtMsgType type, const QMessageLogContext& context, const QString& message) override {
        // TODO: do we need thread safety here?

        LogEntry entry;
        entry.type = type;
        entry.line = context.line;
        entry.file = context.file;
        entry.function = context.function;
        entry.category = context.category;
        entry.message = message;

        m_buffer.emplace_back(std::move(entry));

        if (m_buffer.size() > m_maxSize)
            m_buffer.pop_front();
    }

    void flush(Logger* sink) {
        for (const LogEntry& entry : m_buffer)
            sink->log(entry.type, QMessageLogContext(entry.file, entry.line, entry.function, entry.category), entry.message);

        m_buffer.clear();
    }

private:
    struct LogEntry {
        QtMsgType type = QtInfoMsg;

        /* QMessageLogContext fields. */
        int line = 0;
        const char* file = nullptr;
        const char* function = nullptr;
        const char* category = nullptr;

        QString message;
    };

private:
    size_t m_maxSize = 0;
    std::deque<LogEntry> m_buffer;
};
