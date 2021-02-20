#include "win_platform.h"

#include "com.h"
#include "win_volume_control.h"
#include "win_brightness_control.h"
#include "win_shortcut_dispatcher.h"

WinPlatform::WinPlatform() {
    m_com.reset(new Com());
    m_brightnessControl.reset(new WinBrightnessControl());
    m_shortcutDispatcher.reset(new WinShortcutDispatcher());
}

WinPlatform::~WinPlatform() {
}

PlatformVolumeControl* WinPlatform::createVolumeControl() const {
    return new WinVolumeControl();
}

PlatformBrightnessControl* WinPlatform::brightnessControl() const {
    return m_brightnessControl.get();
}

PlatformShortcutNotifier* WinPlatform::createShortcutNotifier(const QKeySequence& shortcut) const {
    return m_shortcutDispatcher->createShortcutNotifier(shortcut);
}

Platform* createPlatform() {
    return new WinPlatform();
}
