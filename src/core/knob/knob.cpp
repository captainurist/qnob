#include "knob.h"

#include <cassert>

#include <core/entity/entity_creation_context.h>
#include <core/setting/setting.h>

static double clampValue(double value) {
    value = std::clamp(value, 0.0, 1.0);

    if (value < 0.001) {
        value = 0.0;
    } else if (value > 0.999) {
        value = 1.0;
    }

    return value;
}

Knob::~Knob() {}

void Knob::loadFromConfig(const EntityCreationContext& ctx) {
    setStep(ctx.require<double>(lit("step")));
    setSetting(ctx.require<Setting*>(lit("target")));
}

double Knob::step() const {
    return m_step;
}

void Knob::setStep(double step) {
    m_step = step;
}

Setting* Knob::setting() const {
    return m_setting;
}

void Knob::setSetting(Setting* setting) {
    m_setting = setting;
}

void Knob::toggle() {
    if (!m_setting)
        return;

    SettingState state = m_setting->state();
    state.enabled = !state.enabled;
    m_setting->setState(state);

    m_setting->activate();
}

void Knob::decrease() {
    change(-m_step);
}

void Knob::increase() {
    change(m_step);
}

void Knob::change(double delta) {
    if (!m_setting)
        return;

    SettingState state = m_setting->state();
    state.enabled = true;
    state.value = clampValue(state.value + delta);
    m_setting->setState(state);

    m_setting->activate();
}
