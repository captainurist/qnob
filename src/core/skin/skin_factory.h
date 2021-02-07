#pragma once

#include <core/entity/entity_factory.h>

class QPixmap;

class SkinFactory : public EntityFactory {
public:
    SkinFactory();

    virtual Entity* createEntity(const EntityCreationContext& ctx) override;

private:
    static QPixmap requireImage(const EntityCreationContext& ctx, const QString& key);
};
