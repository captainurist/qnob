#pragma once

#include <format>

#include "formatters.h"

template<class... Args>
QString format(QStringView formatString, Args&&... args) {
    QString result;
    detail::QStringToWcharConversionBuffer buffer(formatString);
    detail::QStringBackInsertConvertingProxy resultProxy(&result);
    std::format_to(std::back_inserter(resultProxy), buffer.view(), std::forward<Args>(args)...);
    return result;
}

