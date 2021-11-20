#include "hotkey.h"

#include <platform/platform.h>

#include <lib/metacall/bound_meta_call.h>
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
        BoundMetaCall call;
        call.bind(target, action.toUtf8(), args);

        connect(m_notifier.get(), &PlatformShortcutNotifier::activated, this, [=]() mutable { // TODO: not mutable
            call.invoke();
        });
    }
}



