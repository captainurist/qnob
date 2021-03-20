#pragma once

#include <core/entity/entity.h>

#include <platform/platform_enums.h>

// TODO: probably belongs in a different folder.

class PlatformControl;
class Setting;

class StandardTrayIcon : public Entity {
public:
    StandardTrayIcon(const QString& id, Setting* setting, PlatformStandardControl control);
    virtual ~StandardTrayIcon();

    Setting* setting() const;
    QObject* icon() const;

private:
    void handleSettingChange();

private:
    std::unique_ptr<PlatformControl> m_control;
    Setting* m_setting = nullptr;
};
