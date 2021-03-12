#pragma once

#include <memory>
#include <unordered_map>

#include <QtCore/QObject>
#include <QtGui/QKeySequence>

class PlatformShortcutNotifier;
class WinNativeEventWindow;
class WinShortcutNotifier;

// TODO: WinShortcutManager
class WinShortcutDispatcher: public QObject {
    Q_OBJECT;
public:
    WinShortcutDispatcher(QObject* parent = nullptr);
    ~WinShortcutDispatcher();

    PlatformShortcutNotifier* createShortcutNotifier(const QKeySequence& shortcut);

private:
    void dispatchEvent(void* message);
    void removeShortcutNotifier(int id);
    void convertToNativeKey(Qt::Key key, Qt::KeyboardModifiers mods, quint32* outKey, quint32* outMods) const;

private:
    std::unordered_map<quint32, quint32> m_winKeyByQtKey;
    std::unique_ptr<WinNativeEventWindow> m_eventWindow;
    std::unordered_map<int, WinShortcutNotifier*> m_notifierById;
    int m_nextId = 1;
};
