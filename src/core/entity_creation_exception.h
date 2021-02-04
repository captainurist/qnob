#pragma once

#include <QtCore/QString>

#include <utility/exception.h>

class EntityCreationException : public Exception {
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
