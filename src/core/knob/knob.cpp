#include "knob.h"

void Knob::toggle() {
    setEnabled(!enabled());
}

void Knob::rotate(double delta) {
    setEnabled(true);
    setValue(value() + delta);
}
