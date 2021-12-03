#include "serialization_exceptions.h"

#include <util/exception/bad_cast_exception.h>
#include <util/human_readable.h>
#include <util/format.h>

void throwSerializationException(const std::type_info& type) {
    xthrow BadCastException(sformat(BadCastException::tr("Could not serialize {}"), humanReadableName(type)));
}

void throwDeserializationException(QStringView invalidValue, const std::type_info& typeInfo) {
    xthrow BadCastException(sformat(BadCastException::tr("Could not deserialize value '{}' as {}"), invalidValue, humanReadableName(typeInfo)));
}
