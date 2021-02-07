#pragma once

#include <QtCore/QString>

class Entity;

class FactoryResolver {
public:
    /**
     * \param id                        Id to get entity for.
     * \returns                         Entity for the provided id. Never returns null.
     * \throws EntityCreationException
     */
    virtual Entity* resolveEntity(const QString& id) = 0;

    /**
     * \param path                      Relative path to resolve.
     * \returns                         Absolute path for the provided relative path. Note that returned path
     *                                  might not in fact exist.
     */
    virtual QString resolvePath(const QString& path) = 0;
};
