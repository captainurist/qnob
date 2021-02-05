#pragma once

#include <QtCore/QString>

class Entity;

class EntityResolver {
public:
    /**
     * \param id                        Id to get entity for.
     * \returns                         Entity for the provided id. Never returns null.
     * \throws EntityCreationException
     */
    virtual Entity* resolveEntity(const QString& id) = 0;
};
