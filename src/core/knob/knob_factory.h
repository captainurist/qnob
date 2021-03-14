#pragma once

#include <core/entity/entity_factory.h>

class KnobFactory : public EntityFactory {
public:
    KnobFactory();

    virtual Entity* createEntity(const EntityCreationContext& ctx) override;
};
