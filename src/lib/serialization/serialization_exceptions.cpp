#include "serialization_exceptions.h"

#include <util/bad_cast_exception.h>
#include <util/human_readable_name.h>

void throwSerializationException(const std::type_info& type) {
    qthrow BadCastException(BadCastException::tr("Could not serialize %2").arg(humanReadableName(type)));
}

void throwDeserializationException(QStringView invalidValue, const std::type_info& typeInfo) {
    qthrow BadCastException(BadCastException::tr("Could not deserialize value '%1' as %2").arg(invalidValue).arg(humanReadableName(typeInfo)));
}
