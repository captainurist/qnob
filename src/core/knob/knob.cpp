#include "knob.h"

#include <cassert>

#include "shaft.h"

Knob::Knob(const QString& id, Shaft* shaft) :
    Entity(id),
    m_shaft(shaft)
{
    connect(shaft, &Shaft::notificationReceived, this, &Knob::activateLater);
}

Knob::~Knob() {}

KnobState Knob::state() const {
    KnobState result;
    result.enabled = m_shaft->enabled();
    result.value = m_shaft->value();

    if (result.value == 0.0)
        result.enabled = false;

    return result;
}

void Knob::toggle() {
    m_shaft->setEnabled(!m_shaft->enabled());

    activateLater();
}

void Knob::rotate(double delta) {
    m_shaft->setEnabled(true);
    m_shaft->setValue(m_shaft->value() + delta);

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
