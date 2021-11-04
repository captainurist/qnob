#include "bad_cast_exception.h"

#include "human_readable_name.h"

#include "format.h"

BadCastException::BadCastException(const QString& message) :
    Exception(message)
{}

BadCastException::BadCastException(const std::type_info& from, const std::type_info& to) :
    Exception(sformat(BadCastException::tr("Failed to convert {} to {}"), humanReadableName(from), humanReadableName(to)))
{}

BadCastException::BadCastException(const QMetaType& from, const QMetaType& to):
    Exception(sformat(BadCastException::tr("Failed to convert {} to {}"), QLatin1String(from.name()), QLatin1String(to.name())))
{}
