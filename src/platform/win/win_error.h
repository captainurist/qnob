#pragma once

#include <wtypes.h>
#include <winerror.h>

#include <source_location>
#include <optional>

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

Q_ALWAYS_INLINE std::optional<HANDLE> handleCall(HANDLE result, const char* expr, const std::source_location& location) {
    if (result != NULL && result != INVALID_HANDLE_VALUE) {
        return result;
    } else {
        logError(expr, location);
        return std::nullopt;
    }
}

template<class T, class IsValid>
Q_ALWAYS_INLINE std::optional<T> handleCall(T result, const char* expr, const std::source_location& location, IsValid&& isValid) {
    if (isValid(result)) {
        return result;
    } else {
        logError(expr, location);
        return std::nullopt;
    }
}

}

#define apicall(CALL, ... /* SUCCESS_CHECK */)                                                                          \
    detail::handleCall(CALL, #CALL, std::source_location::current() __VA_OPT__(,) __VA_ARGS__)
