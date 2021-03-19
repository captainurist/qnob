#pragma once

#include <core/entity/entity_factory.h>

class TrayEventFactory : public EntityFactory {
public:
    TrayEventFactory();

    virtual Entity* createEntity(const EntityCreationContext& ctx) override;
};
