#pragma once

#include <memory>

#include <QtCore/QPoint>

#include <core/entity/entity.h>

class OsdWindow;
class OsdFsm;
class Skin;
class KnobState;

class Osd : public Entity {
public:
    Osd(const QString& id);
    virtual ~Osd();

    void show(int showMs, int fadeMs);
    void hide();

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment alignment);

    QPoint offset() const;
    void setOffset(const QPoint& offset);

    // TODO: maybe the three functions below don't belong here?
    // More like setSkin()
    //           setKnob()
    // But then shared ownership is needed? Not really!

    void setSkin(Skin* skin);
    Skin* skin() const;

    KnobState state() const;
    void setState(const KnobState& state);

private:
    void updatePosition();

private:
    std::unique_ptr<OsdWindow> m_window;
    std::unique_ptr<OsdFsm> m_fsm;
    Qt::Alignment m_alignment = Qt::AlignBottom | Qt::AlignHCenter;
    QPoint m_offset = QPoint(0, -100);
};
