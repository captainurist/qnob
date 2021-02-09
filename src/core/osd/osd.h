#pragma once

#include <memory>

#include <QtCore/QPoint>

#include <core/entity/entity.h>

class OsdWindow;
class OsdFsm;
class DeferredPicture;

class Osd : public Entity {
public:
    Osd(const QString& id);
    virtual ~Osd();

    void show(int showMs, int fadeMs);
    void hide();

    QSize size() const;
    void resize(QSize size);

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment alignment);

    QPoint offset() const;
    void setOffset(const QPoint& offset);

    void update(const DeferredPicture& picture);

private:
    void updatePosition();

private:
    std::unique_ptr<OsdWindow> m_window;
    std::unique_ptr<OsdFsm> m_fsm;
    Qt::Alignment m_alignment = Qt::AlignBottom | Qt::AlignHCenter;
    QPoint m_offset = QPoint(0, -100);
};

