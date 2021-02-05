#include "knob.h"

void Knob::toggle() {
    setEnabled(!enabled());
}

void Knob::changeValue(double delta) {
    setEnabled(true);
    setValue(value() + delta);
}
