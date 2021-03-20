#pragma once

#include <QtCore/QScopedPointer>
#include <QtGui/QKeySequence>

#include <core/entity/entity.h>

class PlatformShortcutNotifier;

class Hotkey : public Entity {
    Q_OBJECT
public:
    Hotkey(const QString& id, const QKeyCombination& shortcut);
    virtual ~Hotkey();

signals:
    void triggered();

private:
    QScopedPointer<PlatformShortcutNotifier> m_notifier;
};
