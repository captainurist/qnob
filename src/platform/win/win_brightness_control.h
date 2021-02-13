#pragma once

#include <platform/platform_brightness_control.h>

#include <Windows.h>

class WinBrightnessControl : public PlatformBrightnessControl {
    Q_OBJECT
public:
    WinBrightnessControl();
    virtual ~WinBrightnessControl();

    virtual float brightness() const override;
    virtual void setBrightness(float brightness) override;

    virtual float contrast() const override;
    virtual void setContrast(float contrast) override;

private:
    void monitorProc(HMONITOR handle);

    friend BOOL CALLBACK EnumDisplayMonitorsProc(HMONITOR handle, HDC hdc, LPRECT rect, LPARAM data);

private:
    std::vector<HMONITOR> m_monitorHandles;
};

