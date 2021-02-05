#pragma once

#include <QtCore/QScopedPointer>
#include <QtGui/QKeySequence>

#include "trigger.h"

class PlatformShortcutNotifier;

class HotkeyTrigger : public Trigger {
    Q_OBJECT
public:
    HotkeyTrigger(const QString& id, const QKeySequence& shortcut);
    virtual ~HotkeyTrigger();

private:
    QScopedPointer<PlatformShortcutNotifier> m_notifier;
};
