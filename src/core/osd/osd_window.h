#pragma once

#include <QtGui/QRasterWindow>

#include <core/setting/setting_state.h>

class Skin;

class OsdWindow : public QRasterWindow {
public:
    OsdWindow(const QString& title);

    SettingState state() const;
    void setState(const SettingState& state);

    Skin* skin() const;
    void setSkin(Skin* skin);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    Skin* m_skin = nullptr;
    SettingState m_state;
};
