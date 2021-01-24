#include "win_error.h"

#include <Windows.h>

QString GetLastErrorAsString() {
    DWORD errorMessageID = GetLastError();
    if (errorMessageID == 0)
        return QString();

    LPWSTR messageBuffer = nullptr;
    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&messageBuffer), 0, NULL);

    QString result = QString::fromWCharArray(messageBuffer, size);

    LocalFree(messageBuffer);

    return result;
}
