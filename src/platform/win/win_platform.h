#pragma once

#include <memory>

#include <platform/platform.h>

class QThread;

class Com;
class WinVolumeControl;
class WinMonitorManager;
class WinTrayIconWheelEventManager;
class WinShortcutManager;
class WinGlobalMouseHook;

class WinPlatform: public Platform {
    Q_OBJECT
public:
    WinPlatform();
    virtual ~WinPlatform();

    virtual PlatformVolumeControl* volumeControl() const override;
    virtual PlatformMonitorManager* monitorManager() const override;
    virtual PlatformTrayIconWheelEventManager* trayIconWheelEventManager() const override;
    virtual PlatformShortcutNotifier* createShortcutNotifier(const QKeySequence& shortcut) const override;
    virtual bool hooksEnabled() const override;
    virtual void setHooksEnabled(bool enabled) override;

private:
    Q_SIGNAL void hookChangeRequested(bool enabled);

private:
    std::unique_ptr<Com> m_com;
    WinGlobalMouseHook* m_hook = nullptr;
    std::unique_ptr<QThread> m_hookThread;
    std::unique_ptr<WinVolumeControl> m_volumeControl;
    std::unique_ptr<WinMonitorManager> m_monitorManager;
    std::unique_ptr<WinTrayIconWheelEventManager> m_trayIconWheelEventManager;
    std::unique_ptr<WinShortcutManager> m_shortcutDispatcher;
};

