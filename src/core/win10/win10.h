#pragma once

#include <core/entity/entity.h>

class Win10 : public Entity {
    Q_OBJECT
public:
    using Entity::Entity;
    virtual ~Win10();

    virtual void initialize(const EntityCreationContext& ctx) override;

    Q_INVOKABLE void ensureNativeOsdHidden();

private:
    bool m_systemOsdHidden = false;
};
