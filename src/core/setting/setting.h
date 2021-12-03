#pragma once

#include <memory>

#include <core/entity/entity.h>

#include "setting_state.h"

class SettingBackend;

class Setting : public Entity {
    Q_OBJECT
public:
    Setting(std::unique_ptr<SettingBackend> backend, QObject* parent);
    virtual ~Setting();

    bool isInitialized() const; // TODO: isStarted?

    SettingState state() const;
    void setState(const SettingState& state);

    void activate();

signals:
    void activated();
    void changed();

private:
    void updateCachedState();

private:
    std::unique_ptr<SettingBackend> m_backend;
    SettingState m_cachedState;
};
