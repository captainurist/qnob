#include "win_brightness_control.h"

#include <highlevelmonitorconfigurationapi.h>

#include "win_physical_monitor.h"

class WinBrightnessControlPrivate {
public:
    std::unique_ptr<WinPhysicalMonitor> monitor;
    DWORD minBrightness;
    DWORD maxBrightness;
    DWORD currentBrightness;

    float mapToFloat(DWORD brightness) const {
        return 1.0f * (brightness - minBrightness) / maxBrightness;
    }

    DWORD mapFromFloat(float brightness) const {
        return minBrightness + std::clamp(brightness, 0.0f, 1.0f) * (maxBrightness - minBrightness);
    }
};

WinBrightnessControl::WinBrightnessControl() {
    for (auto& physicalMonitor : WinPhysicalMonitor::enumerateMonitors()) {
        WinBrightnessControlPrivate& monitorData = m_monitorData.emplace_back();

        monitorData.monitor = std::move(physicalMonitor);
        monitorData.monitor->readBrightness(&monitorData.minBrightness, &monitorData.currentBrightness, &monitorData.maxBrightness);
    }
}

WinBrightnessControl::~WinBrightnessControl() {
}

float WinBrightnessControl::brightness() const {
    return m_monitorData[0].mapToFloat(m_monitorData[0].currentBrightness);
}

void WinBrightnessControl::setBrightness(float brightness) {
    for (auto& monitorData : m_monitorData) {
        DWORD physicalBrightness = monitorData.mapFromFloat(brightness);
        monitorData.monitor->setBrightness(physicalBrightness);
        monitorData.currentBrightness = physicalBrightness;
    }
}

float WinBrightnessControl::contrast() const {
    return 0.0f;
}

void WinBrightnessControl::setContrast(float contrast) {

}


