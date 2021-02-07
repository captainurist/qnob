#pragma once

#include <memory>

#include <core/entity/entity.h>

class OsdWindow;
class DeferredPicture;

class Osd : public Entity {
public:
    Osd(const QString& id);
    virtual ~Osd();

    void show(int showMs, int fadeMs);
    void hide();

    void resize(QSize size);
    void move(QPoint point);

    void update(const DeferredPicture& picture);

private:
    std::unique_ptr<OsdWindow> m_window;
};

