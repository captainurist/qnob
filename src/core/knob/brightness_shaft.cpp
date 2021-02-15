#include "brightness_shaft.h"

#include <platform/platform.h>

BrightnessShaft::BrightnessShaft() {
    m_brightnessControl = platform()->brightnessControl();
}

BrightnessShaft::~BrightnessShaft() {}

double BrightnessShaft::value() const {
    return m_brightnessControl->brightness();
}

void BrightnessShaft::setValue(double value) {
    m_brightnessControl->setBrightness(value);
}

bool BrightnessShaft::enabled() const {
    return true;
}

void BrightnessShaft::setEnabled(bool /*value*/) {
    return; /* Just ignore it for now. */
}

