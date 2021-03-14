#pragma once

#include <memory>

#include <core/entity/entity.h>

class Setting;

class Knob : public Entity {
    Q_OBJECT
public:
    Knob(const QString& id);
    virtual ~Knob();

    double step() const;
    void setStep(double step);

    Setting* setting() const;
    void setSetting(Setting* setting);

    Q_INVOKABLE void toggle();
    Q_INVOKABLE void decrease();
    Q_INVOKABLE void increase();

private:
    void change(double delta);

private:
    double m_step = 0.0;
    Setting* m_setting = nullptr;
};

