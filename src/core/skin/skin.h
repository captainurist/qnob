#pragma once

#include <QtGui/QPixmap>

#include <core/entity/entity.h>

class KnobState;
class DeferredPicture;

class SkinData {
public:
    QPixmap background;
    QPixmap progress;
    QPixmap disabled;
    QPoint offset;
    int units = 0;
};

class Skin : public Entity {
    Q_OBJECT
public:
    Skin(const QString& id, const SkinData& data);

    QSize size() const;

    DeferredPicture picture(const KnobState& state) const;

private:
    SkinData m_data;
    QSize m_size;
};
