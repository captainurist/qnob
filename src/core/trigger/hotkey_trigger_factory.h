#pragma once

#include <QtCore/QString>

#include <core/entity/entity_factory.h>

class HotkeyTriggerFactory : public EntityFactory {
    Q_OBJECT
public:
    HotkeyTriggerFactory();

    virtual Entity* createEntity(const EntityConfig& config, EntityResolver* resolver) override;

private:
    template<class T>
    static T requireData(const EntityConfig& config, const QString& key);

    template<class T>
    static T requireDataOr(const EntityConfig& config, const QString& key, const T& defaultValue);

    template<class T>
    static T requireDataInternal(const EntityConfig& config, const QString& key, const T* defaultValue = nullptr);
};
