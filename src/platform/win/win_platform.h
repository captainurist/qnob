#pragma once

#include <memory>

#include <platform/platform.h>

class Com;
class WinVolumeControl;
class WinBrightnessControl;
class WinShortcutDispatcher;

class WinPlatform: public Platform {
public:
    WinPlatform();
    virtual ~WinPlatform();

    virtual PlatformVolumeControl* createVolumeControl() const override;
    virtual PlatformMonitorManager* createMonitorManager() const override;
    virtual PlatformShortcutNotifier* createShortcutNotifier(const QKeySequence& shortcut) const override;
    virtual PlatformTrayIconWheelEventManager* createTrayIconWheelEventManager() const override;

private:
    std::unique_ptr<Com> m_com;
    std::unique_ptr<WinShortcutDispatcher> m_shortcutDispatcher;
};

