#pragma once

#include <utility/exception.h>

class EntityCreationException : public Exception {
    Q_DECLARE_TR_FUNCTIONS(EntityCreationException)
public:
    EntityCreationException(const QString& id, const QString& message) :
        Exception(message),
        m_id(id)
    {}

    const QString& id() const {
        return m_id;
    }

private:
    QString m_id;
};
