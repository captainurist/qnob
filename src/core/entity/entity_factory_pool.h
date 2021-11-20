#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

#include <QtCore/QObject>
#include <QtCore/QHashFunctions>
#include <QtCore/QString>

class Entity;

using EntityFactoryFunction = std::function<std::unique_ptr<Entity>(QObject*)>;

class EntityFactoryPool: public QObject {
    Q_OBJECT
public:
    EntityFactoryPool(QObject* parent);
    virtual ~EntityFactoryPool();

    /**
     * Registers an entity factory. This pool must not contain a factory with the same id.
     *
     * \param typeId                    Entity type id.
     * \param factoryFunction           Entity factory function.
     */
    void registerFactory(const QString& typeId, const EntityFactoryFunction& factoryFunction);

    // TODO: docs
    template<class EntityClass>
    void registerSimpleFactory(const QString& typeId) {
        registerFactory(typeId, [](QObject* parent) { return std::make_unique<EntityClass>(parent); });
    }

    /**
     * \param typeId                    Entity type id.
     * \returns                         Factory registered for the provided type id, or an empty factory if none
     *                                  was registered.
     */
    const EntityFactoryFunction& factory(const QString& typeId) const;

private:
    std::unordered_map<QString, EntityFactoryFunction> m_factoryFunctionByTypeId;
    EntityFactoryFunction m_emptyFactory;
};
