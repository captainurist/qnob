#include "bad_cast_exception.h"

BadCastException::BadCastException(const QString& message) :
    Exception(message)
{}

/*
BadCastException::BadCastException(const std::type_info& from, const std::type_info& to);
BadCastException::BadCastException(const QMetaType& from, const QMetaType& to);
*/
//TODO
