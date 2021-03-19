#pragma once

#include <core/entity/entity_factory.h>

class HotkeyFactory : public EntityFactory {
public:
    HotkeyFactory();

    virtual Entity* createEntity(const EntityCreationContext& ctx) override;
};
