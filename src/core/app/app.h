#pragma once

#include <core/entity/entity.h>

class App : public Entity {
    Q_OBJECT
public:
    App(const QString& id);

    Q_SLOT void exit();
    Q_SLOT void debugAction();
};
