#pragma once

#include <platform/platform_control.h>

class WinStandardControl : public PlatformControl {
    Q_OBJECT
public:
    WinStandardControl(PlatformStandardControl control, QObject* parent);
    virtual ~WinStandardControl();

    virtual QRect geometry() const override;
    virtual PlatformStandardControl standardType() const override;

private:
    PlatformStandardControl m_control;
};
