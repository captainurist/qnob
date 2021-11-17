#include "serialization_exceptions.h"

#include <util/exception/bad_cast_exception.h>
#include <util/human_readable.h>

void throwSerializationException(const std::type_info& type) {
    xthrow BadCastException(BadCastException::tr("Could not serialize %2").arg(humanReadableName(type)));
}

void throwDeserializationException(QStringView invalidValue, const std::type_info& typeInfo) {
    xthrow BadCastException(BadCastException::tr("Could not deserialize value '%1' as %2").arg(invalidValue).arg(humanReadableName(typeInfo)));
}
