#pragma once

#include <core/entity/entity_factory.h>

class Win10Factory : public EntityFactory {
public:
    Win10Factory();

    virtual Entity* createEntity(const EntityCreationContext& ctx) override;
};
