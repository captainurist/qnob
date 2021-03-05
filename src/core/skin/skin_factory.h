#pragma once

#include <core/entity/entity_factory.h>

#include "skin_type.h"


class SkinFactory : public EntityFactory {
public:
    SkinFactory(SkinType type);

    virtual Entity* createEntity(const EntityCreationContext& ctx) override;

private:
    SkinType m_type;
};
