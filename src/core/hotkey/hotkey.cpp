#include "hotkey.h"

#include <platform/platform.h>

#include <lib/metacall/meta_closure.h>
#include <lib/keys/key_combination.h>

#include <core/entity/entity_config.h>

Hotkey::~Hotkey() {}

void Hotkey::loadFromConfig(const EntityConfig& cfg) {
    QKeyCombination shortcut = cfg.require<QKeyCombination>(lit("trigger"));
    Entity* target = cfg.require<Entity*>(lit("target"));
    QString action = cfg.require<QString>(lit("action"));
    QVariantList args = cfg.requireOr<QVariantList>(lit("args"), QVariantList());

    m_notifier = platform()->shortcutManager()->createShortcutNotifier(shortcut, this);
    if (m_notifier) {
        connect(m_notifier.get(), &PlatformShortcutNotifier::activated, target, [closure = MetaClosure(target, action.toUtf8(), args)] {
            closure.invoke();
        });
    }
}



