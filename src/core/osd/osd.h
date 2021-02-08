#pragma once

#include <memory>

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

    QPoint position() const;
    void setPosition(QPoint point);

    void update(const DeferredPicture& picture);

private:
    std::unique_ptr<OsdWindow> m_window;
    std::unique_ptr<OsdFsm> m_fsm;
};

