#pragma once

#include <memory>

#include <QtCore/QPoint>

#include <core/entity/entity.h>

class QScreen;
class OsdWindow;
class OsdFsm;
class Skin;
class Setting;

class Osd : public Entity {
public:
    Osd(QObject* parent);
    virtual ~Osd();

    void show(int showMs, int fadeMs);
    void hide();

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment alignment);

    QPoint offset() const;
    void setOffset(const QPoint& offset);

    Skin* skin() const;
    void setSkin(Skin* skin);

    Setting* setting() const;
    void setSetting(Setting* setting);

protected:
    virtual void loadFromConfig(const EntityConfig& cfg) override;

private:
    void updatePosition();
    void updatePrimaryScreen();

private:
    std::unique_ptr<OsdWindow> m_window;
    std::unique_ptr<OsdFsm> m_fsm;
    Qt::Alignment m_alignment = Qt::AlignBottom | Qt::AlignHCenter;
    QPoint m_offset = QPoint(0, -100);
    Setting* m_setting = nullptr;
    QScreen* m_primaryScreen = nullptr;
};
