#pragma once

#include <core/entity/entity.h>
#include <core/setting/setting_state.h>

class QSystemTrayIcon;
class Skin;
class Setting;

class TrayIcon : public Entity {
    Q_OBJECT
public:
    TrayIcon(const QString& id);
    virtual ~TrayIcon();

    Skin* skin() const;
    void setSkin(Skin* skin);

    Setting* setting() const;
    void setSetting(Setting* setting);

private:
    void updateIcon();

private:
    std::unique_ptr<QSystemTrayIcon> m_trayIcon;
    Skin* m_skin = nullptr;
    Setting* m_setting = nullptr;
};
