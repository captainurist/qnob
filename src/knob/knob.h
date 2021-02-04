#pragma once

#include <core/entity.h>

class Knob : public Entity {
    Q_OBJECT
public:
    Knob(const QString& id) : Entity(id) {}

    virtual double value() const = 0;
    virtual void setValue(double value) = 0;

    virtual bool enabled() const = 0;
    virtual void setEnabled(bool value) = 0;

signals:
    void valueChanged(double value);
    void enabledChanged(bool value);
};

