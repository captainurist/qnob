#include "win_shortcut_manager.h"

#include <Windows.h>

#include <array>

#include <QtCore/QPointer>
#include <QtCore/QDebug>
#include <QtGui/QWindow>

#include <platform/platform_shortcut_notifier.h>
#include <util/map_access.h>

#include "win_error.h"
#include "win_shortcut_notifier.h"
#include "win_shared_event_window.h"
#include "win_shortcut_key_table.h"

WinShortcutManager::WinShortcutManager(WinSharedEventWindow* eventWindow) :
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

PlatformShortcutNotifier* WinShortcutManager::createShortcutNotifier(const QKeyCombination& shortcut) const {
    Qt::Key key = shortcut.key();
    Qt::KeyboardModifiers mods = shortcut.keyboardModifiers();
    quint32 nativeKey = 0;
    quint32 nativeMods = 0;
    convertToNativeKey(key, mods, &nativeKey, &nativeMods);
    if (nativeKey == 0) {
        qWarning() << "Could not convert" << shortcut << "to native key combination.";
        return nullptr;
    }

    if (!apicall(RegisterHotKey(reinterpret_cast<HWND>(m_eventWindow->winId()), m_nextId, nativeMods, nativeKey)))
        return nullptr;

    WinShortcutNotifier* notifier = new WinShortcutNotifier(m_nextId);
    connect(notifier, &WinShortcutNotifier::aboutToBeDestroyed, this, &WinShortcutManager::removeShortcutNotifier);
    m_notifierById[m_nextId] = notifier;
    m_nextId++;
    return notifier;
}

void WinShortcutManager::dispatchEvent(MSG* message) {
    assert(message->message == WM_HOTKEY);

    WinShortcutNotifier* notifier = value_or(m_notifierById, message->wParam);
    if (!notifier) {
        qWarning() << "Received hotkey message w/o notifier, lParam = " << message->lParam << ", wParam = " << message->wParam;
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

