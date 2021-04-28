#pragma once

#include <wtypes.h>

#include <atomic>

#include <QtCore/QObject>

typedef struct tagMSLLHOOKSTRUCT MSLLHOOKSTRUCT;

// TODO: saner API here. Thread-safe setEnabled, hooking / unhooking on thread change,
// always handling events on object's thread.

// TODO: maybe rewrite with raw input?

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
