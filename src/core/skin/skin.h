#pragma once

#include <core/entity/entity.h>

class QPainter;
class SettingState;

class Skin : public Entity {
    Q_OBJECT
public:
    Skin(const QString& id) : Entity(id) {}

    virtual QSize size() const = 0;
    virtual void paint(QPainter* painter, const SettingState& state) = 0;
};
