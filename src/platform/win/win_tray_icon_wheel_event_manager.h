#pragma once

#include <Windows.h>

#include <memory>
#include <unordered_set>

#include <platform/platform_tray_icon_wheel_event_manager.h>

class QSystemTrayIcon;

class WinTrayIconWheelEventManager: public PlatformTrayIconWheelEventManager {
    Q_OBJECT
public:
    WinTrayIconWheelEventManager();
    virtual ~WinTrayIconWheelEventManager();

    virtual void registerTrayIcon(QSystemTrayIcon* icon) override;
    virtual void unregisterTrayIcon(QSystemTrayIcon* icon) override;

    virtual void registerStandardIcon(PlatformStandardTrayIcon standardIcon, QObject* icon);
    virtual void unregisterStandardIcon(PlatformStandardTrayIcon standardIcon);

private:
    Q_SLOT void processMessage(UINT message, const MSLLHOOKSTRUCT& data);

private:
    std::unique_ptr<QThread> m_hookThread;
    std::unordered_set<QSystemTrayIcon*> m_icons;
    std::unordered_map<PlatformStandardTrayIcon, QObject*> m_standardIcons;
};
