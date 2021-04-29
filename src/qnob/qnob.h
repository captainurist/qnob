#pragma once

#include <memory>

class BufferLogger;
class FileLogger;
class QFile;

class Qnob {
public:
    Qnob();
    ~Qnob();

    int run(int argc, char** argv);

private:
    std::unique_ptr<QFile> m_logFile;
    std::unique_ptr<BufferLogger> m_bufferLogger;
    std::unique_ptr<FileLogger> m_fileLogger;
};
