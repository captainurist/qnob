#pragma once

#include <core/entity/entity_factory.h>

class OsdFactory: public EntityFactory {
public:
    OsdFactory();

    virtual Entity* createEntity(const EntityCreationContext& ctx) override;
};
