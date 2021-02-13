#include "exception.h"

#include <QtCore/QDebug>
#include <QtCore/QThread>

#include <util/human_readable_name.h>

static QString currentThreadOutputName() {
    if (QString result = QThread::currentThread()->objectName(); !result.isEmpty())
        return QLatin1Char('\'') + result + QLatin1Char('\'');

    return lit("0x%1").arg(reinterpret_cast<std::intptr_t>(QThread::currentThreadId()), 8, 16, QLatin1Char('0'));
}

static void printException(QDebug& stream, const std::exception& e) {
    stream
        << humanReadableName(typeid(e))
        << " in thread "
        << currentThreadOutputName()
        << ": "
        << e.what()
        << Qt::endl;
}

static void printException(QDebug& stream, const Exception& e) {
    stream
        << humanReadableName(typeid(e))
        << " in thread "
        << currentThreadOutputName()
        << ": "
        << e.message()
        << Qt::endl;

    stream
        << "        at "
        << humanReadableSignature(e.sourceLocation().function_name())
        << "("
        << e.sourceLocation().file_name()
        << ":"
        << e.sourceLocation().line()
        << ")";

    if (!e.cause())
        return;

    try {
        std::rethrow_exception(e.cause());
    } catch (const Exception& cause) {
        stream << Qt::endl << "Caused by ";
        printException(stream, cause);
    } catch (const std::exception& cause) {
        stream << Qt::endl << "Caused by ";
        printException(stream, cause);
    } catch (...) {
        stream << Qt::endl << "Caused by unknown exception.";
    }
}

QDebug&& operator<<(QDebug&& stream, const Exception& e) {
    QDebugStateSaver saver(stream);
    stream.noquote().nospace();

    printException(stream, e);

    return std::forward<QDebug>(stream);
}
