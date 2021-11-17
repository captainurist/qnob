#pragma once

#include <util/exception/exception.h>

class EntityCreationException : public Exception {
    Q_DECLARE_TR_FUNCTIONS(EntityCreationException)
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
        return tr("Failed to create entity '%1': %2").arg(id).arg(message);
    }

private:
    QString m_id;
};
