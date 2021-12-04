#pragma once

#include <util/exception/exception.h>
#include <util/format.h>

class EntityCreationException : public Exception {
    X_DECLARE_TR_FUNCTIONS(EntityCreationException)
public:
    EntityCreationException(const QString& id, const QString& message) :
        Exception(fullMessage(id, message)),
        m_id(id)
    {}

    const QString& id() const {
        return m_id;
    }

private:
    QString fullMessage(const QString& id, const QString& message) {
        return sformat(tr("Failed to create entity '{}': {}"), id, message);
    }

private:
    QString m_id;
};
