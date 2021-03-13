#include "knob.h"

#include <cassert>

#include "shaft.h"

static double clampValue(double value) {
    value = std::clamp(value, 0.0, 1.0);

    if (value < 0.001) {
        value = 0.0;
    } else if (value > 0.999) {
        value = 1.0;
    }

    return value;
}


Knob::Knob(const QString& id, Shaft* shaft) :
    Entity(id),
    m_shaft(shaft)
{
    connect(shaft, &Shaft::changedExternally, this, &Knob::activateLater);
}

Knob::~Knob() {}

KnobState Knob::state() const {
    KnobState result;
    result.enabled = m_shaft->isEnabled();
    result.value = m_shaft->value();
    return result;
}

void Knob::toggle() {
    m_shaft->setEnabled(!m_shaft->isEnabled());

    activateLater();
}

void Knob::rotate(double delta) {
    double value = m_shaft->value();
    if (std::isnan(value))
        return; /* Shaft is either broken or pending initialization. */

    m_shaft->setEnabled(true);
    m_shaft->setValue(clampValue(value + delta));

    activateLater();
}

void Knob::activateLater() {
    if (m_activationPending)
        return;

    m_activationPending = true;

    auto callback = [&] {
        assert(m_activationPending);
        m_activationPending = false;

        emit activated();
    };
    QMetaObject::invokeMethod(this, callback, Qt::QueuedConnection);
}

