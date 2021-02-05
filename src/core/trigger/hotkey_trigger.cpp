#include "hotkey_trigger.h"

#include <platform/platform.h>

HotkeyTrigger::HotkeyTrigger(const QString& id, const QKeySequence& shortcut):
    Trigger(id)
{
    m_notifier.reset(qPlatform()->createShortcutNotifier(shortcut));
    if (!m_notifier)
        return;

    connect(m_notifier.get(), &PlatformShortcutNotifier::activated, this, &Trigger::triggered);
}

HotkeyTrigger::~HotkeyTrigger() {}


