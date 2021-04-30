#pragma once

#include <memory>

class QFile;
class QApplication;
class BufferLogger;
class FileLogger;
class QnobArgs;

class Qnob {
public:
    Qnob();
    ~Qnob();

    int run(int argc, char** argv);

private:
    int run(const QnobArgs& args);
    int runHelp();
    int runVersion();
    int runList(const QnobArgs& args);
    int runService(const QnobArgs& args);

private:
    std::unique_ptr<QFile> m_logFile;
    std::unique_ptr<BufferLogger> m_bufferLogger;
    std::unique_ptr<FileLogger> m_fileLogger;
    std::unique_ptr<QApplication> m_application;
};
