#pragma once

#include <guiddef.h>

#include <platform/platform_control.h>

class WinTrayIconControl : public PlatformControl {
    Q_OBJECT
public:
    WinTrayIconControl(const GUID& guid, QObject* parent);
    virtual ~WinTrayIconControl();

    virtual QRect geometry() const override;
    virtual bool isVisible() const override;
    virtual void setVisible(bool visible) override;

private:
    GUID m_guid;
};
