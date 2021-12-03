#include "win_error.h"

#include <Windows.h>

#include <cassert>
#include <type_traits>

#include <QtCore/QDebug>

#include <util/debug.h>

static QString GetLastErrorAsString() {
    DWORD errorMessageID = GetLastError();
    if (errorMessageID == 0)
        return QString();

    LPWSTR messageBuffer = nullptr;
    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&messageBuffer), 0, NULL);

    QString result = QString::fromWCharArray(messageBuffer, size);

    LocalFree(messageBuffer);

    while (result.endsWith(QLatin1Char('\n')) || result.endsWith(QLatin1Char('\r')))
        result.chop(1);

    return result;
}

static QByteArray extractFunctionName(const char* expr) {
    QByteArray result = expr;

    qsizetype pos = result.indexOf('(');
    if (pos == -1)
        return result;
    result.resize(pos);

    pos = result.indexOf('>'); /* Handling object->Method syntax. */
    if (pos == -1)
        return result;
    result = result.mid(pos + 1);

    return result;
}

void detail::logError(const char* expr, const std::source_location& location) {
    QString lastErrorString = GetLastErrorAsString();
    if (lastErrorString.isEmpty())
        lastErrorString = lit("<unknown error>");

    xMessageOutput(
        QtWarningMsg,
        location,
        nullptr,
        L"{} failed: {}",
        QLatin1String(extractFunctionName(expr)),
        lastErrorString
    );
}

void detail::logError(HRESULT result, const char* expr, const std::source_location& location) {
    assert(!SUCCEEDED(result));

    xMessageOutput(
        QtWarningMsg,
        location,
        nullptr,
        L"{} failed with HRESULT={:#0X}",
        QLatin1String(extractFunctionName(expr)),
        static_cast<std::make_unsigned_t<HRESULT>>(result)
    );
}

