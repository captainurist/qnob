#pragma once

#include <Windows.h>

#include <platform/platform_control.h>

class WinHwndControl : public PlatformControl {
    Q_OBJECT
public:
    WinHwndControl(HWND hwnd, QObject* parent);
    virtual ~WinHwndControl();

    virtual QRect geometry() const override;
    virtual bool isMinimized() const override;
    virtual void setMinimized(bool minimized) override;

private:
    HWND m_hwnd;
};
