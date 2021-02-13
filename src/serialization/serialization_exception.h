#pragma once

#include <any>

#include <util/human_readable_name.h>
#include <util/exception.h>


class SerializationException : public Exception {
    Q_DECLARE_TR_FUNCTIONS(SerializationException)
public:
    SerializationException(std::any value, const QString& message = QString()) :
        Exception(message.isEmpty() ? defaultExceptionMessage(value.type()) : message),
        m_value(std::move(value))
    {}

    const std::any& value() const {
        return m_value;
    }

private:
    static QString defaultExceptionMessage(const std::type_info& typeInfo) {
        return tr("Could not serialize %2").arg(humanReadableName(typeInfo));
    }

private:
    std::any m_value;
};
