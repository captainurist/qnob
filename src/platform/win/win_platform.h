#pragma once

#include <platform/platform.h>

#include <QtCore/QScopedPointer>

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
    QScopedPointer<Com> m_com;
    QScopedPointer<WinVolumeControl> m_volumeControl;
    QScopedPointer<WinShortcutDispatcher> m_shortcutDispatcher;
};

