#pragma once

#include "exception.h"

class BadCastException : public Exception {
    X_DECLARE_TR_FUNCTIONS(BadCastException)
public:
    BadCastException(const QString& message);
    BadCastException(const std::type_info& from, const std::type_info& to);
    BadCastException(const QMetaType& from, const QMetaType& to);

    /* Accessors make little sense here, so none are provided. */
};
