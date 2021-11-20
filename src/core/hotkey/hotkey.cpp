#include "hotkey.h"

#include <platform/platform.h>

#include <lib/metacall/bound_meta_call.h>
#include <lib/keys/key_combination.h>

#include <core/entity/entity_creation_context.h>

Hotkey::~Hotkey() {}

void Hotkey::loadFromConfig(const EntityCreationContext& ctx) {
    QKeyCombination shortcut = ctx.require<QKeyCombination>(lit("trigger"));
    Entity* target = ctx.require<Entity*>(lit("target"));
    QString action = ctx.require<QString>(lit("action"));
    QVariantList args = ctx.requireOr<QVariantList>(lit("args"), QVariantList());

    m_notifier = platform()->shortcutManager()->createShortcutNotifier(shortcut, this);
    if (m_notifier) {
        BoundMetaCall call;
        call.bind(target, action.toUtf8(), args);

        connect(m_notifier.get(), &PlatformShortcutNotifier::activated, this, [=]() mutable { // TODO: not mutable
            call.invoke();
        });
    }
}



