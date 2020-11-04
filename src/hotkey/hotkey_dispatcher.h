#pragma once

#include <QtCore/QScopedPointer>

#include "hotkey_notifier.h"
#include "hotkey.h"

class HotkeyDispatcherPrivate;

class HotkeyDispatcher {
public:
    static HotkeyDispatcher* instance();

    HotkeyNotifier* registerHotkey(const Hotkey& hotkey);
    void unregisterHotkey(const Hotkey& hotkey);

private:
    HotkeyDispatcher();
    ~HotkeyDispatcher();

private:
    QScopedPointer<HotkeyDispatcherPrivate> d;
};