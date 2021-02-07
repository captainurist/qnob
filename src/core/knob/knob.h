#pragma once

#include <core/entity/entity.h>

class Knob : public Entity {
    Q_OBJECT
public:
    Knob(const QString& id) : Entity(id) {}

    virtual double value() const = 0;
    virtual void setValue(double value) = 0;

    virtual bool enabled() const = 0;
    virtual void setEnabled(bool value) = 0;

    /* Functions available through actions. */

    Q_INVOKABLE void toggle();
    Q_INVOKABLE void rotate(double delta);

signals:
    void touched();
    void valueChanged(double value);
    void enabledChanged(bool value);
};
