#pragma once

#include <QtCore/QObject>

class Entity : public QObject {
    Q_OBJECT
public:
    Entity(const QString& id): m_id(id) {}

    const QString& id() const {
        return m_id;
    }

private:
    QString m_id;
};
