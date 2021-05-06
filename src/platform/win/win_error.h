#pragma once

#include <wtypes.h>
#include <winerror.h>

#include <source_location>

#include <QtCore/QString>

QString GetLastErrorAsString();

namespace detail {

void logError(HRESULT result, const char* expr, const std::source_location& location);
void logError(const char* expr, const std::source_location& location);

Q_ALWAYS_INLINE bool handleCall(BOOL result, const char* expr, const std::source_location& location) {
    if (!result)
        logError(expr, location);
    return result;
}

Q_ALWAYS_INLINE bool handleCall(HRESULT result, const char* expr, const std::source_location& location) {
    if (!SUCCEEDED(result))
        logError(result, expr, location);
    return SUCCEEDED(result);
}

Q_ALWAYS_INLINE HHOOK handleCall(HHOOK result, const char* expr, const std::source_location& location) {
    if (!result)
        logError(expr, location);
    return result;
}

}

#define apicall(CALL) detail::handleCall(CALL, #CALL, std::source_location::current())
