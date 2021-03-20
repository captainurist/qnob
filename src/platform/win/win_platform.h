#pragma once

#include <memory>

#include <platform/platform.h>

class QThread;

class Com;
class WinVolumeControl;
class WinMonitorManager;
class WinWheelEventManager;
class WinShortcutManager;
class WinGlobalMouseHook;

class WinPlatform: public Platform {
    Q_OBJECT
public:
    WinPlatform();
    virtual ~WinPlatform();

    virtual PlatformVolumeControl* volumeControl() const override;
    virtual PlatformMonitorManager* monitorManager() const override;
    virtual PlatformShortcutManager* shortcutManager() const override;
    virtual PlatformWheelEventManager* trayIconWheelEventManager() const override;
    virtual PlatformControl* createStandardControl(PlatformStandardControl control) const override;
    virtual void execute(PlatformFunction function) override;

private:
    Q_SIGNAL void hookChangeRequested(bool enabled);
    void updateCurrentToolTip() const;

private:
    std::unique_ptr<Com> m_com;
    WinGlobalMouseHook* m_hook = nullptr;
    std::unique_ptr<QThread> m_hookThread;
    std::unique_ptr<WinVolumeControl> m_volumeControl;
    std::unique_ptr<WinMonitorManager> m_monitorManager;
    std::unique_ptr<WinWheelEventManager> m_trayIconWheelEventManager;
    std::unique_ptr<WinShortcutManager> m_shortcutManager;
};

