#pragma once

#include <QtCore/QString>

#include <core/entity_factory.h>

class HotkeyTriggerFactory : public EntityFactory {
    Q_OBJECT
public:
    virtual Entity* createEntity(const EntityConfig& config, EntityResolver* resolver) override;

private:
    template<class T>
    static T requireData(const EntityConfig& config, const QString& key);
};
