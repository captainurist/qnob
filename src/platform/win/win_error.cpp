#include "win_error.h"

#include <Windows.h>

#include <cassert>
#include <type_traits>

#include <QtCore/QDebug>

QString GetLastErrorAsString() {
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

    pos = result.indexOf('>');
    if (pos == -1)
        return result;
    result = result.mid(pos + 1);

    return result;
}

void detail::logError(BOOL result, const char* expr, const std::source_location& location) {
    assert(!result);
    Q_UNUSED(result);

    QString lastErrorString = GetLastErrorAsString();

    QMessageLogger(location.file_name(), location.line(), location.function_name()).warning().noquote().nospace()
        << extractFunctionName(expr)
        << " failed: "
        << (lastErrorString.isEmpty() ? lit("<unknown error>") : lastErrorString);
}

void detail::logError(HRESULT result, const char* expr, const std::source_location& location) {
    assert(!SUCCEEDED(result));

    QMessageLogger(location.file_name(), location.line(), location.function_name()).warning().noquote().nospace()
        << extractFunctionName(expr)
        << " failed with HRESULT=0x"
        << lit("%1").arg(static_cast<std::make_unsigned_t<HRESULT>>(result), sizeof(result) * 2, 16, QLatin1Char('0'));
}

