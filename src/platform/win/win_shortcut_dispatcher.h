#pragma once

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QScopedPointer>
#include <QtGui/QKeySequence>

class PlatformShortcutNotifier;
class WinShortcutEventHandler;

class WinShortcutDispatcher: public QObject {
    Q_OBJECT;
public:
    WinShortcutDispatcher(QObject* parent = nullptr);
    ~WinShortcutDispatcher();

    PlatformShortcutNotifier* createShortcutNotifier(const QKeySequence& shortcut);

private:
    friend class WinShortcutEventHandler;
    friend class WinShortcutNotifier;

    void dispatchEvent(void* message);
    void removeShortcutNotifier(int id);
    void convertToNativeKey(Qt::Key key, Qt::KeyboardModifiers mods, quint32* outKey, quint32* outMods) const;

private:
    QHash<quint32, quint32> m_winKeyByQtKey;
    QScopedPointer<WinShortcutEventHandler> m_eventHandler;
    QHash<int, WinShortcutNotifier*> m_notifierById;
    int m_nextId = 1;
};
