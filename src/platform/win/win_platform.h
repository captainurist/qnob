#pragma once

#include <memory>

#include <platform/platform.h>

class Com;
class WinVolumeControl;
class WinShortcutDispatcher;

class WinPlatform: public Platform {
public:
    WinPlatform();
    virtual ~WinPlatform();

    virtual PlatformVolumeControl* volumeControl() const override;
    virtual PlatformShortcutNotifier* createShortcutNotifier(const QKeySequence& shortcut) const override;

private:
    std::unique_ptr<Com> m_com;
    std::unique_ptr<WinVolumeControl> m_volumeControl;
    std::unique_ptr<WinShortcutDispatcher> m_shortcutDispatcher;
};

