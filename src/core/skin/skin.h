#pragma once

#include <QtGui/QPixmap>

#include <core/entity/entity.h>

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

private:
    SkinData m_data;
};
