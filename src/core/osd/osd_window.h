#pragma once

#include <QtGui/QRasterWindow>

#include <core/knob/knob_state.h>

class Skin;

class OsdWindow : public QRasterWindow {
public:
    OsdWindow(const QString& title);

    KnobState state() const;
    void setState(const KnobState& state);

    Skin* skin() const;
    void setSkin(Skin* skin);

protected:
    virtual void paintEvent(QPaintEvent* paintEvent) override;

private:
    Skin* m_skin = nullptr;
    KnobState m_state;
};
