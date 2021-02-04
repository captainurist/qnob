#pragma once

#include <core/entity.h>

class Trigger : public Entity {
    Q_OBJECT
public:
    Trigger(const QString& id) : Entity(id) {}

signals:
    void triggered();
};
