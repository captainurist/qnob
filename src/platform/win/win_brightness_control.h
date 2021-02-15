#pragma once

#include <platform/platform_brightness_control.h>

class WinBrightnessControlPrivate;

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
    std::vector<WinBrightnessControlPrivate> m_monitorData;
};

