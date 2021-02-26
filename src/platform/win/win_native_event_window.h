#pragma once

#include <WinUser.h>

#include <QtGui/QWindow>

class WinNativeEventWindow : public QWindow {
    Q_OBJECT
public:
    WinNativeEventWindow(const QString& title, UINT message) :
        m_message(message)
    {
        setTitle(title); /* Mainly used for debugging with Spy++. */
    }

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
