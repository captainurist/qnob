#pragma once

#include <Windows.h>

#include <memory>
#include <unordered_set>

#include <platform/platform_tray_icon_wheel_event_manager.h>

class QSystemTrayIcon;
class WinGlobalMouseHook;

class WinTrayIconWheelEventManager: public PlatformTrayIconWheelEventManager {
    Q_OBJECT
public:
    WinTrayIconWheelEventManager(WinGlobalMouseHook* hook);
    virtual ~WinTrayIconWheelEventManager();

    virtual void registerTrayIcon(QSystemTrayIcon* icon) override;
    virtual void unregisterTrayIcon(QSystemTrayIcon* icon) override;

    virtual void registerStandardIcon(PlatformStandardTrayIcon standardIcon, QObject* icon) override;
    virtual void unregisterStandardIcon(PlatformStandardTrayIcon standardIcon, QObject* icon) override;

private:
    Q_SLOT void processMessage(UINT message, const MSLLHOOKSTRUCT& data);

private:
    std::unordered_set<QSystemTrayIcon*> m_icons;
    std::unordered_map<PlatformStandardTrayIcon, std::unordered_set<QObject*>> m_objectsByStandardIcon;
};
