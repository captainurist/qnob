#pragma once

#include <core/entity/entity.h>

#include <platform/platform_enums.h>

// TODO: probably belongs in a different folder.

class StandardTrayIcon : public Entity {
public:
    StandardTrayIcon(const QString& id, PlatformStandardTrayIcon standardIcon);
    virtual ~StandardTrayIcon();

    QObject* icon() const;

private:
    PlatformStandardTrayIcon m_standardIcon;
};
