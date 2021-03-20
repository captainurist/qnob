#pragma once

#include <core/entity/entity.h>

#include <platform/platform_enums.h>

#include <core/setting/setting.h>

// TODO: probably belongs in a different folder.

class StandardTrayIcon : public Entity {
public:
    StandardTrayIcon(const QString& id, Setting* setting, PlatformStandardTrayIcon standardIcon);
    virtual ~StandardTrayIcon();

    Setting* setting() const;
    QObject* icon() const;

private:
    void handleSettingChange();

private:
    Setting* m_setting = nullptr;
    PlatformStandardTrayIcon m_standardIcon;
};
