#pragma once

#include <memory>

#include <core/entity/entity.h>

#include "knob_state.h"

class Shaft;

class Knob : public Entity {
    Q_OBJECT
public:
    Knob(const QString& id, Shaft* shaft);
    virtual ~Knob();

    KnobState state() const;

    Q_INVOKABLE void toggle();
    Q_INVOKABLE void rotate(double delta);

signals:
    /**
     * Emitted whenever an action on this knob is performed, even if it didn't lead to a state change.
     */
    void activated();

private:
    void activateLater();

    static double clampValue(double value);

private:
    std::unique_ptr<Shaft> m_shaft;
    bool m_activationPending = false;
};

