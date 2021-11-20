#pragma once

#include <core/entity/entity.h>

class Win10 : public Entity {
    Q_OBJECT
public:
    using Entity::Entity;
    virtual ~Win10();

    Q_INVOKABLE void ensureNativeOsdHidden();

protected:
    virtual void loadFromConfig(const EntityCreationContext& ctx) override;

private:
    bool m_systemOsdHidden = false;
};
