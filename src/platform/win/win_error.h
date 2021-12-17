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

template<class T, class SuccessCheck>
Q_ALWAYS_INLINE T handleCall(T result, const char* expr, const std::source_location& location, SuccessCheck&& check) {
    if (!check(result))
        logError(expr, location);
    return result;
}

}

#define apicall(CALL, ... /* SUCCESS_CHECK */)                                                                          \
    detail::handleCall(CALL, #CALL, std::source_location::current() __VA_OPT__(,) __VA_ARGS__)
