#pragma once

#include <core/entity/entity_factory.h>

class SoundFactory : public EntityFactory {
public:
    SoundFactory();

    virtual Entity* createEntity(const EntityCreationContext& ctx) override;
};
