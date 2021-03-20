#pragma once

#include <Windows.h>

#include <QtCore/QObject>

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
    bool m_enabled = false;
};
