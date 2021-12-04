#pragma once

#include <util/exception/exception.h>

// TODO

class FutureContextException : public Exception {
public:
    FutureContextException() :
        Exception(QString())
    {}
};
