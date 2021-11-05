#pragma once

#include <platform/platform_shortcut_notifier.h>

class WinShortcutNotifier : public PlatformShortcutNotifier {
    Q_OBJECT
public:
    WinShortcutNotifier(int id, QObject* parent): PlatformShortcutNotifier(parent), m_id(id) {}

    ~WinShortcutNotifier() {
        emit aboutToBeDestroyed(m_id);
    }

signals:
    void aboutToBeDestroyed(int id);

private:
    int m_id = 0;
};
