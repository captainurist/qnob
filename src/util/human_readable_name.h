#pragma once

#include <QtCore/QString>
#include <QtCore/QMetaType>

/**
 * Creates a human-readable name from a `type_info` object.
 */
QString humanReadableName(const std::type_info& typeInfo);

template<class T>
QString humanReadableName() {
    return humanReadableName(typeid(T));
}

QString humanReadableSignature(const char* signature);
