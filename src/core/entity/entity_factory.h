#pragma once

#include <QtCore/QObject>

#include "entity_creation_context.h"

class EntityFactory {
public:
    EntityFactory(const QString& id) : m_id(id) {}

    virtual Entity* createEntity(const EntityCreationContext& ctx) = 0;

    const QString& id() const {
        return m_id;
    }

private:
    QString m_id;
};
