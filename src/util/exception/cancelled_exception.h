#pragma once

#include "exception.h"

/**
 * Generic exception to signal cancellation, e.g. when a class holding a `Promise` gets destroyed and there is
 * nothing to write
 */
class CancelledException : public Exception {
    X_DECLARE_TR_FUNCTIONS(CancelledException)
public:
    CancelledException();
    CancelledException(const QString& message);
};
