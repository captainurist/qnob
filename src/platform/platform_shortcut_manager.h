#pragma once

#include <unordered_set>

#include <QtCore/QObject>

class PlatformShortcutNotifier;

QT_SPECIALIZE_STD_HASH_TO_CALL_QHASH_BY_VALUE(QKeyCombination)

class PlatformShortcutManager : public QObject {
    Q_OBJECT
public:
    PlatformShortcutManager(QObject* parent) : QObject(parent) {}

    /**
     * \returns                         A set of keys that can be used as hotkeys.
     *                                  The only allowed modifier for returned keys is `Qt::KeypadModifier`.
     */
    virtual std::unordered_set<QKeyCombination> bindableKeys() const = 0;

    /**
     * \param shortcut                  Key combination to create a global shortcut for.
     * \returns                         New shortcut notifier, or `nullptr` in case of an error.
     */
    virtual std::unique_ptr<PlatformShortcutNotifier> createShortcutNotifier(const QKeyCombination& shortcut, QObject* parent) const = 0;
};
