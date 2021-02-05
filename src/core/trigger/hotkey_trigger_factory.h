#pragma once

#include <core/entity/entity_factory.h>

class HotkeyTriggerFactory : public EntityFactory {
public:
    HotkeyTriggerFactory();

    virtual Entity* createEntity(const EntityConfig& config, EntityResolver* resolver) override;
};
