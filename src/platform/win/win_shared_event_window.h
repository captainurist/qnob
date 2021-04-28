#pragma once

#include <QtGui/QWindow>

typedef struct tagMSG MSG;

class WinSharedEventWindow : public QWindow {
    Q_OBJECT
public:
    WinSharedEventWindow();

signals:
    /**
     * WM_HOTKEY signal.
     */
    void hotkey(MSG* message);

    /**
     * WM_DISPLAYCHANGE signal.
     */
    void displayChange(MSG* message);

    /**
     * "TaskbarCreated" custom message.
     */
    void taskbarCreated(MSG* message);

protected:
    virtual bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
};
