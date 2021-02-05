#include "knob.h"

void Knob::toggle() {
    setEnabled(!enabled());
}

void Knob::changeValue(double delta) {
    setValue(value() + delta);
}
