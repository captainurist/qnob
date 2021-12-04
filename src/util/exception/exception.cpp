#include "exception.h"

#include <QtCore/QDebug>
#include <QtCore/QThread>

#include <util/human_readable.h>
#include <util/format.h>
#include <util/source_root.h>

static QString currentThreadOutputName() {
    if (QString result = QThread::currentThread()->objectName(); !result.isEmpty())
        return QLatin1Char('\'') + result + QLatin1Char('\'');

    return sformat(L"{:#016x}", reinterpret_cast<std::intptr_t>(QThread::currentThreadId()));
}

static void printException(QDebug& stream, const std::exception& e) {
    stream
        << humanReadableName(typeid(e))
        << ": "
        << e.what()
        << Qt::endl;
}

static void printException(QDebug& stream, const Exception& e, bool printThread) {
    stream << humanReadableName(typeid(e));

    if (printThread)
        stream << " in thread " << currentThreadOutputName();

    stream
        << ": "
        << e.message()
        << Qt::endl;

    stream
        << "        at "
        << humanReadableSignature(e.sourceLocation().function_name())
        << "("
        << QByteArrayView(rootRelativeSourcePath(e.sourceLocation().file_name()))
        << ":"
        << e.sourceLocation().line()
        << ")";

    if (!e.cause())
        return;

    try {
        std::rethrow_exception(e.cause());
    } catch (const Exception& cause) {
        stream << Qt::endl << "Caused by ";
        printException(stream, cause, false);
    } catch (const std::exception& cause) {
        stream << Qt::endl << "Caused by ";
        printException(stream, cause);
    } catch (...) {
        stream << Qt::endl << "Caused by unknown exception.";
    }
}

Exception::Exception(const QString& message, ChainMode chainMode) {
    m_message = message;
    m_utf8Message = message.toUtf8();
    if (chainMode == AutoChain)
        m_cause = std::current_exception();
}

char const* Exception::what() const {
    return m_utf8Message.data();
}


QDebug operator<<(QDebug stream, const Exception& e) {
    QDebugStateSaver saver(stream);
    stream.noquote().nospace();

    printException(stream, e, true);

    return stream;
}
