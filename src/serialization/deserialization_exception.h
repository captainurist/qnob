#pragma once

#include <typeinfo>

#include <util/human_readable_name.h>
#include <util/exception.h>

class DeserializationException : public Exception {
    Q_DECLARE_TR_FUNCTIONS(DeserializationException)
public:
    DeserializationException(const QString& invalidValue, const std::type_info& typeInfo, const QString& message = QString()) :
        Exception(message.isEmpty() ? defaultExceptionMessage(invalidValue, typeInfo) : message),
        m_invalidValue(invalidValue),
        m_typeInfo(&typeInfo)
    {}

    const QString& invalidValue() const {
        return m_invalidValue;
    }

    const std::type_info& typeInfo() const {
        return *m_typeInfo;
    }

private:
    static QString defaultExceptionMessage(const QString& invalidValue, const std::type_info& typeInfo) {
        return tr("Could not deserialize value '%1' as %2").arg(invalidValue).arg(humanReadableName(typeInfo));
    }

private:
    QString m_invalidValue;
    const std::type_info* m_typeInfo;
};
