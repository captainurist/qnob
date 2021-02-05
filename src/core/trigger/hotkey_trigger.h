#pragma once

#include <QtCore/QScopedPointer>
#include <QtGui/QKeySequence>

#include "trigger.h"

class PlatformShortcutNotifier;

class HotkeyTrigger : public Trigger {
    Q_OBJECT
public:
    HotkeyTrigger(const QKeySequence& shortcut, const QString& id);
    virtual ~HotkeyTrigger();

private:
    QScopedPointer<PlatformShortcutNotifier> m_notifier;
};
