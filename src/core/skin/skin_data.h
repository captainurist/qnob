#pragma once

#include <QtGui/QPixmap>

class SkinData {
public:
    QPixmap background;
    QPixmap progress;
    QPixmap disabled;
    QPoint offset;
    int units = 0;
};
