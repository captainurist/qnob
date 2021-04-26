#pragma once

#include <Windows.h> /* For MSLLHOOKSTRUCT. */

#include <atomic>

#include <QtCore/QObject>

// TODO: saner API here. Thread-safe setEnabled, hooking / unhooking on thread change,
// always handling events on object's thread.

/**
 * Global mouse hook for mouse wheel events.
 *
 * Should be created in a dedicated thread to make sure that the hook function returns ASAP.
 */
class WinGlobalMouseHook : public QObject {
    Q_OBJECT
public:
    WinGlobalMouseHook();
    virtual ~WinGlobalMouseHook();

    bool isEnabled();
    void setEnabled(bool enabled);

signals:
    void messageHooked(UINT message, const MSLLHOOKSTRUCT& data);

private:
    std::atomic<bool> m_enabled = false;
};
