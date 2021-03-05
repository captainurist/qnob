#pragma once

#include <QtGui/QRasterWindow>

#include <core/knob/knob_state.h>

class KnobPainter;

class OsdWindow : public QRasterWindow {
public:
    OsdWindow(const QString& title);

    KnobState state() const;
    void setState(const KnobState& state);

    void setPainter(KnobPainter* painter);

protected:
    virtual void paintEvent(QPaintEvent* paintEvent) override;

private:
    std::unique_ptr<KnobPainter> m_painter;
    KnobState m_state;
};
