#pragma once

#include <memory>

#include <core/entity/entity.h>

class PlatformShortcutNotifier;

class Hotkey : public Entity {
    Q_OBJECT
public:
    using Entity::Entity;
    virtual ~Hotkey();

protected:
    virtual void loadFromConfig(const EntityCreationContext& ctx) override;

private:
    std::unique_ptr<PlatformShortcutNotifier> m_notifier;
};
