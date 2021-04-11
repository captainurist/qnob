#pragma once

#include <QtCore/QStringView>

#include <lib/serialization/serialization.h>

template<class T>
void parseOptionValue(QStringView value, T* target) {
    deserialize(value, target);
}
