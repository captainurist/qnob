#pragma once

#include <wtypes.h>
#include <winerror.h>

#include <QtCore/QString>

#include <util/source_location.h>

QString GetLastErrorAsString();

namespace detail {

void logError(HRESULT result, const char* expr, const std::source_location& location);
void logError(BOOL result, const char* expr, const std::source_location& location);

Q_ALWAYS_INLINE bool isSuccess(BOOL result) {
    return result;
}

Q_ALWAYS_INLINE bool isSuccess(HRESULT result) {
    return SUCCEEDED(result);
}

}

// TODO: rename apicall()
#define succeeded(CALL) (                                                                                               \
    [&] {                                                                                                               \
        auto __result = CALL;                                                                                           \
        if (detail::isSuccess(__result))                                                                                \
            return true;                                                                                                \
        detail::logError(__result, #CALL, __LOCATION__);                                                                \
        return false;                                                                                                   \
    }                                                                                                                   \
)()
