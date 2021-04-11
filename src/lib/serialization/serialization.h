#pragma once

#include <QtCore/QString>
#include <QtCore/QStringView>

#include "builtin_serialization.h"

template<class T>
inline QString serialize(const T& value) {
    QString result;
    serialize(value, &result);
    return result;
}

template<class T>
inline T deserialize(QStringView string) {
    T result;
    deserialize(string, &result);
    return result;
}
