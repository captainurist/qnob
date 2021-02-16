#include "bad_cast_exception.h"

#include "human_readable_name.h"

BadCastException::BadCastException(const QString& message) :
    Exception(message)
{}

BadCastException::BadCastException(const std::type_info& from, const std::type_info& to) :
    Exception(BadCastException::tr("Failed to convert %1 to %2").arg(humanReadableName(from)).arg(humanReadableName(to)))
{}

BadCastException::BadCastException(const QMetaType& from, const QMetaType& to):
    Exception(BadCastException::tr("Failed to convert %1 to %2").arg(QLatin1String(from.name())).arg(QLatin1String(to.name())))
{}
