#pragma once

#include <WinUser.h>

#include <QtGui/QWindow>

class WinNativeEventWindow : public QWindow {
    Q_OBJECT
public:
    WinNativeEventWindow(UINT message) : m_message(message) {}

signals:
    void messageReceived(void* message);

protected:
    virtual bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override {
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == m_message)
            emit messageReceived(message);

        return QWindow::nativeEvent(eventType, message, result);
    }

public:
    UINT m_message = WM_NULL;
};
