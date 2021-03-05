#pragma once

class KnobState {
public:
    bool enabled = false;
    double value = 0.0;

    friend bool operator==(const KnobState& l, const KnobState& r) = default;
};
