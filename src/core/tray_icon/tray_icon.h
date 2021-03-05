#pragma once

#include <core/entity/entity.h>
#include <core/knob/knob_state.h>

class QSystemTrayIcon;
class Skin;

class TrayIcon : public Entity {
    Q_OBJECT
public:
    TrayIcon(const QString& id);
    virtual ~TrayIcon();

    Skin* skin() const;
    void setSkin(Skin* skin);

    KnobState state() const;
    void setState(const KnobState& state);

private:
    void updateIcon();

private:
    std::unique_ptr<QSystemTrayIcon> m_trayIcon;
    Skin* m_skin;
    KnobState m_state;
};
