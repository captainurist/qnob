#include "knob.h"

void Knob::toggle() {
    setEnabled(!enabled());

    emit touched(); // TODO
}

void Knob::rotate(double delta) {
    setEnabled(true);
    setValue(value() + delta);

    emit touched(); // TODO
}
