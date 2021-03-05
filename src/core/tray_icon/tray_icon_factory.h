#pragma once

#include <core/entity/entity_factory.h>

class TrayIconFactory : public EntityFactory {
public:
    TrayIconFactory();

    virtual Entity* createEntity(const EntityCreationContext& ctx) override;
};
