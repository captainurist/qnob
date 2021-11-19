#pragma once

#include <core/entity/entity.h>

class App : public Entity {
    Q_OBJECT
public:
    using Entity::Entity;

    Q_SLOT void exit();
    Q_SLOT void debugAction();
};
