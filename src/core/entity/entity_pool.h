#pragma once

#include <unordered_map>
#include <memory>

#include <QtCore/QObject>
#include <QtCore/QHashFunctions>
#include <QtCore/QString>

#include <util/variant.h>

class Entity;
class EntityFactoryPool;

class EntityPool: public QObject {
    Q_OBJECT
public:
    EntityPool(QObject* parent);
    virtual ~EntityPool();

    /**
     * Adds provided entity into this entity pool. This pool must not contain an entity with the same id.
     *
     * \param entity                    Entity to add.
     */
    void addEntity(std::unique_ptr<Entity> entity);

    void loadEntities(const QString& basePath, const EntityFactoryPool* factoryPool, const std::unordered_map<QString, VariantMap>& configs);

    Entity* entity(const QString& id) const;

    std::vector<Entity*> entities() const;

    void clear();

private:
    std::unordered_map<QString, std::unique_ptr<Entity>> m_entityById;
};
