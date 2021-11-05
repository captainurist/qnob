#include "win_shortcut_manager.h"

#include <Windows.h>

#include <array>

#include <QtCore/QPointer>
#include <QtGui/QWindow>

#include <platform/platform_shortcut_notifier.h>
#include <util/map_access.h>
#include <util/debug.h>

#include "win_error.h"
#include "win_shortcut_notifier.h"
#include "win_shared_event_window.h"
#include "win_shortcut_key_table.h"

WinShortcutManager::WinShortcutManager(WinSharedEventWindow* eventWindow, QObject* parent) :
    PlatformShortcutManager(parent),
    m_eventWindow(eventWindow)
{
    for (quint32 winKey = 0; winKey < KeyMapping.size(); winKey++) {
        int qtKey = KeyMapping[winKey].key;
        switch (qtKey) {
        case -1:
        case 0:
        case Qt::Key_Meta:
        case Qt::Key_Shift:
        case Qt::Key_Alt:
        case Qt::Key_Control:
            continue;
        }

        QKeyCombination combinedKey = static_cast<Qt::Key>(qtKey) | KeyMapping[winKey].mods;

        assert(!m_winKeyByQtKey.contains(combinedKey.toCombined()));
        m_winKeyByQtKey[combinedKey.toCombined()] = winKey;

        m_bindableKeys.insert(combinedKey);
    }

    connect(m_eventWindow, &WinSharedEventWindow::hotkey, this, &WinShortcutManager::dispatchEvent);
}

WinShortcutManager::~WinShortcutManager() {}

std::unordered_set<QKeyCombination> WinShortcutManager::bindableKeys() const {
    return m_bindableKeys;
}

std::unique_ptr<PlatformShortcutNotifier> WinShortcutManager::createShortcutNotifier(const QKeyCombination& shortcut, QObject* parent) const {
    Qt::Key key = shortcut.key();
    Qt::KeyboardModifiers mods = shortcut.keyboardModifiers();
    quint32 nativeKey = 0;
    quint32 nativeMods = 0;
    convertToNativeKey(key, mods, &nativeKey, &nativeMods);
    if (nativeKey == 0) {
        xWarning("Could not convert {} to native key combination", shortcut);
        return nullptr;
    }

    // TODO: need a hotkey in log, and maybe even a desktop notification!
    if (!apicall(RegisterHotKey(reinterpret_cast<HWND>(m_eventWindow->winId()), m_nextId, nativeMods, nativeKey)))
        return nullptr;

    std::unique_ptr<WinShortcutNotifier> notifier = std::make_unique<WinShortcutNotifier>(m_nextId, parent);
    connect(notifier.get(), &WinShortcutNotifier::aboutToBeDestroyed, this, &WinShortcutManager::removeShortcutNotifier);
    m_notifierById[m_nextId] = notifier.get();
    m_nextId++;
    return notifier;
}

void WinShortcutManager::dispatchEvent(MSG* message) {
    assert(message->message == WM_HOTKEY);

    WinShortcutNotifier* notifier = value_or(m_notifierById, message->wParam);
    if (!notifier) {
        xWarning("Received hotkey message w/o notifier, lParam = {}, wParam = {}", message->lParam, message->wParam);
        return;
    }

    emit notifier->activated();
}

void WinShortcutManager::removeShortcutNotifier(int id) {
    apicall(UnregisterHotKey(reinterpret_cast<HWND>(m_eventWindow->winId()), id));

    m_notifierById.erase(id);
}

void WinShortcutManager::convertToNativeKey(Qt::Key key, Qt::KeyboardModifiers mods, quint32* outKey, quint32* outMods) const {
    int winMods = 0;
    if (mods & Qt::ShiftModifier)
        winMods |= MOD_SHIFT;
    if (mods & Qt::ControlModifier)
        winMods |= MOD_CONTROL;
    if (mods & Qt::AltModifier)
        winMods |= MOD_ALT;
    if (mods & Qt::MetaModifier)
        winMods |= MOD_WIN;

    int qtKey = key;
    if (mods & Qt::KeypadModifier)
        qtKey |= Qt::KeypadModifier;

    *outKey = value_or(m_winKeyByQtKey, qtKey, 0);
    *outMods = winMods;
}

