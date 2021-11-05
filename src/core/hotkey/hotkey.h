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
    std::unique_ptr<PlatformShortcutNotifier> m_notifier;
};
