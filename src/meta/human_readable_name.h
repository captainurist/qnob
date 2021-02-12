#pragma once

#include <QtCore/QString>
#include <QtCore/QMetaType>

template<class T>
QString humanReadableName() {
    return QLatin1String(QMetaType::fromType<T>().name());
}

#define QB_DECLARE_HUMAN_READABLE_NAME(TYPE, NAME)                                                                      \
template<>                                                                                                              \
QString humanReadableName<TYPE>() {                                                                                     \
    return QLatin1String(NAME);                                                                                         \
}
