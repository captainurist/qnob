#include "hotkey_trigger.h"

#include <QtGui/QWindow>
#include <QtCore/QDebug>

#include <windows.h>

QString GetLastErrorAsString() {
    DWORD errorMessageID = GetLastError();
    if (errorMessageID == 0)
        return QString();

    LPWSTR messageBuffer = nullptr;
    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&messageBuffer), 0, NULL);

    QString result = QString::fromWCharArray(messageBuffer, size);

    LocalFree(messageBuffer);

    return result;
}

class HotkeyTriggerPrivate : public QWindow {
public:
    void init() {
        BOOL ok = RegisterHotKey(reinterpret_cast<HWND>(winId()), 1, MOD_ALT, VK_PAUSE);
        QString s = GetLastErrorAsString();
        qDebug() << s;
    }

    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result) override {
        Q_UNUSED(eventType);
        Q_UNUSED(result);
        
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == WM_HOTKEY) {
            if (msg->wParam == 1) {
                // We inform about this to the console
                //qDebug() << "HotKey worked";
                return true;
            }
        }
        return false;
    }

public:
    HotkeyTrigger* q = nullptr;
};

HotkeyTrigger::HotkeyTrigger(QObject* parent) : 
    Trigger(parent),
    d(new HotkeyTriggerPrivate)
{
    d->q = this;
    d->init();
}

HotkeyTrigger::~HotkeyTrigger() {

}
