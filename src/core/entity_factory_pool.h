#pragma once

#include <memory>
#include <unordered_map>

#include <QtCore/QHashFunctions>
#include <QtCore/QString>

class EntityFactory;

class EntityFactoryPool {
public:
    ~EntityFactoryPool();

    /**
     * Registers an entity factory. This pool must not contain a factory with the same id.
     *
     * Note that this pool takes ownership of the provided factory.
     *
     * \param id                        Type id that the provided entity factory will handle.
     * \param factory                   Entity factory.
     */
    void registerFactory(const QString& id, EntityFactory* factory);

    EntityFactory* factory(const QString& id) const;

private:
    std::unordered_map<QString, std::unique_ptr<EntityFactory>> m_factoryById;
};
