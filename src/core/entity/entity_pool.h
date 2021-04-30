#pragma once

#include <unordered_map>
#include <memory>

#include <QtCore/QHashFunctions>
#include <QtCore/QString>

class Entity;

class EntityPool {
public:
    ~EntityPool();

    /**
     * Adds provided entity into this entity pool. This pool must not contain an entity with the same id.
     *
     * The pool takes ownership of the provided entity.
     *
     * \param entity                    Entity to add.
     */
    void addEntity(Entity* entity);

    Entity* entity(const QString& id) const;

    std::vector<Entity*> entities() const;

private:
    std::unordered_map<QString, std::unique_ptr<Entity>> m_entityById;
};
