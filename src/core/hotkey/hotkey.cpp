#include "hotkey.h"

#include <platform/platform.h>

Hotkey::Hotkey(const QString& id, const QKeyCombination& shortcut):
    Entity(id)
{
    m_notifier = platform()->shortcutManager()->createShortcutNotifier(shortcut, this);
    if (!m_notifier)
        return;

    connect(m_notifier.get(), &PlatformShortcutNotifier::activated, this, &Hotkey::triggered);
}

Hotkey::~Hotkey() {}


