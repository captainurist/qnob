#include "win_brightness_control.h"

#include <highlevelmonitorconfigurationapi.h>

/*
BOOL CALLBACK EnumDisplayMonitorsProc(HMONITOR handle, HDC / *hdc* /, LPRECT / *rect* /, LPARAM data) {
    reinterpret_cast<WinBrightnessControl*>(data)->monitorProc(handle);
    return TRUE;
}
*/

WinBrightnessControl::WinBrightnessControl() {
    //EnumDisplayMonitors(nullptr, nullptr, EnumDisplayMonitorsProc, reinterpret_cast<LPARAM>(this));
}

WinBrightnessControl::~WinBrightnessControl() {

}

float WinBrightnessControl::brightness() const {
    return 0.0f;
}

void WinBrightnessControl::setBrightness(float brightness) {
    /*for(size_t i = 0; i < m_monitorHandles.size(); i++)
        SetMonitorBrightness(m_monitorHandles[i], );*/
}

float WinBrightnessControl::contrast() const {
    return 0.0f;
}

void WinBrightnessControl::setContrast(float contrast) {

}

void WinBrightnessControl::monitorProc(HMONITOR handle) {
    m_monitorHandles.push_back(handle);
}

