#pragma once

#include <wtypes.h>

#include <QtCore/QString>

#include <util/source_location.h>

QString GetLastErrorAsString();

namespace detail {

bool checkAndLog(BOOL result, const char* call, const std::source_location& location);
bool checkAndLog(HRESULT result, const char* call, const std::source_location& location);

}

#define succeeded(CALL) (detail::checkAndLog(CALL, #CALL, __LOCATION__))
