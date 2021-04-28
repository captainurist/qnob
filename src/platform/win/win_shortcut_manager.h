#pragma once

#include <memory>
#include <unordered_map>

#include <platform/platform_shortcut_manager.h>

typedef struct tagMSG MSG;
class PlatformShortcutNotifier;
class WinSharedEventWindow;
class WinShortcutNotifier;

class WinShortcutManager: public PlatformShortcutManager {
    Q_OBJECT;
public:
    WinShortcutManager(WinSharedEventWindow* eventWindow);
    ~WinShortcutManager();

    virtual std::unordered_set<QKeyCombination> bindableKeys() const override;
    virtual PlatformShortcutNotifier* createShortcutNotifier(const QKeyCombination& shortcut) const override;

private:
    void dispatchEvent(MSG* message);
    void removeShortcutNotifier(int id);
    void convertToNativeKey(Qt::Key key, Qt::KeyboardModifiers mods, quint32* outKey, quint32* outMods) const;

private:
    std::unordered_set<QKeyCombination> m_bindableKeys;
    std::unordered_map<int, quint32> m_winKeyByQtKey;
    WinSharedEventWindow* m_eventWindow = nullptr;
    mutable std::unordered_map<int, WinShortcutNotifier*> m_notifierById;
    mutable int m_nextId = 1;
};
