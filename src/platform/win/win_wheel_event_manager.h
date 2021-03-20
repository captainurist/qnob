#pragma once

#include <Windows.h>

#include <memory>
#include <unordered_set>

#include <platform/platform_wheel_event_manager.h>

class QSystemTrayIcon;
class WinGlobalMouseHook;
class PlatformControl;

class WinWheelEventManager: public PlatformWheelEventManager {
    Q_OBJECT
public:
    WinWheelEventManager(WinGlobalMouseHook* hook);
    virtual ~WinWheelEventManager();

    virtual void registerTrayIcon(QSystemTrayIcon* icon) override;
    virtual void unregisterTrayIcon(QSystemTrayIcon* icon) override;

    virtual void registerControl(PlatformControl* control) override;
    virtual void unregisterControl(PlatformControl* control) override;

private:
    Q_SLOT void processMessage(UINT message, const MSLLHOOKSTRUCT& data);

private:
    std::unordered_set<QSystemTrayIcon*> m_icons;
    std::unordered_set<PlatformControl*> m_controls;
};
