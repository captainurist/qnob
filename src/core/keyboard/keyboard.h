#pragma once

#include <core/entity/entity.h>

class Keyboard : public Entity {
    Q_OBJECT
public:
    Keyboard(QObject* parent);

    Q_INVOKABLE void type(const QString& text);
};
