#include "brightness_setting_backend.h"

#include <cassert>

#include <lib/monitor/monitor_manager.h>

BrightnessSettingBackend::BrightnessSettingBackend(MonitorManager* monitorManager):
    m_monitorManager(monitorManager)
{
    connect(m_monitorManager, &MonitorManager::monitorsChanged, this, &BrightnessSettingBackend::handleMonitorsChanged);
}

BrightnessSettingBackend::~BrightnessSettingBackend() {}

double BrightnessSettingBackend::value() const {
    if (!m_initialized)
        return NAN;

    if (m_cachedValue)
        return *m_cachedValue;

    double result = 0;
    for (auto& monitor : m_monitorManager->monitors())
        result += monitor->property(BrightnessMonitorProperty);
    result /= m_monitorManager->monitors().size();

    assert(!std::isnan(result));

    m_cachedValue = result;
    return result;
}

void BrightnessSettingBackend::setValue(double value) {
    assert(value >= 0.0 && value <= 1.0);

    if (!m_initialized)
        return;

    m_cachedValue = value;
    for (auto& monitor : m_monitorManager->monitors())
        monitor->setProperty(BrightnessMonitorProperty, value);
}

bool BrightnessSettingBackend::isEnabled() const {
    return true;
}

void BrightnessSettingBackend::setEnabled(bool /*value*/) {
    return; /* Not supported by the underlying API. */
}

bool BrightnessSettingBackend::isInitialized() const {
    return m_initialized;
}

void BrightnessSettingBackend::handleMonitorsChanged() {
    if (m_initialized)
        return;

    if (m_monitorManager->monitors().empty())
        return;

    m_initialized = true;
    emit initialized();
}
