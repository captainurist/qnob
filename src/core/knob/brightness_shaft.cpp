#include "brightness_shaft.h"

#include <cmath>

#include <platform/platform.h>

BrightnessShaft::BrightnessShaft() :
    m_monitorManager(platform()->createMonitorManager()),
    m_monitors(m_monitorManager->enumerateMonitors()),
    m_value(NAN)
{}

BrightnessShaft::~BrightnessShaft() {}

double BrightnessShaft::value() const {
    double result = m_value;

    if (std::isnan(result)) {
        result = 0;
        for (auto& monitor : m_monitors)
            result += monitor->property(PlatformMonitor::BrightnessProperty);
        result /= m_monitors.size();

        if (std::isnan(result))
            result = 0.0;
    }

    return result;
}

void BrightnessShaft::setValue(double value) {
    m_value = value;
    for (auto& monitor : m_monitors)
        monitor->setProperty(PlatformMonitor::BrightnessProperty, value);
}

bool BrightnessShaft::enabled() const {
    return true;
}

void BrightnessShaft::setEnabled(bool /*value*/) {
    return; /* Just ignore it for now. */
}

