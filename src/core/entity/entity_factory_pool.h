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
     * \param factory                   Entity factory.
     */
    void registerFactory(std::unique_ptr<EntityFactory> factory);

    EntityFactory* factory(const QString& id) const;

private:
    std::unordered_map<QString, std::unique_ptr<EntityFactory>> m_factoryById;
};
