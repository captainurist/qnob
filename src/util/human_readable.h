#pragma once

#include <QtCore/QString>
#include <QtCore/QMetaType>

/**
 * \param typeInfo                      Type info object.
 * \returns                             Human-readable type name for the provided type info object.
 */
QString humanReadableName(const std::type_info& typeInfo);

template<class T>
QString humanReadableName() {
    return humanReadableName(typeid(T));
}

/**
 * \param signature                     Method or functions signature as returned by `__FUNCTION__`.
 * \returns                             Human-readable signature.
 */
QString humanReadableSignature(const char* signature);
