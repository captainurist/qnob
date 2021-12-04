#pragma once

#include <util/exception/exception.h>

class CommandLineException : public Exception {
    X_DECLARE_TR_FUNCTIONS(CommandLineException)
public:
    CommandLineException(const QString& message) : Exception(message) {}
};
