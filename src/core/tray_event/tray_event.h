#pragma once

#include <core/entity/entity.h>

#include "mouse_key.h"

class QWheelEvent;
class QSystemTrayIcon;

class TrayEvent : public Entity {
    Q_OBJECT
public:
    TrayEvent(const QString& id);
    virtual ~TrayEvent();

    void setEventSource(QObject* eventSource);
    QObject* eventSource() const;

    void setKey(MouseKey key);
    MouseKey key() const;

signals:
    void triggered();

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    bool wheelEvent(QWheelEvent* event);

private:
    MouseKey m_key = Key_WheelUp;
    QObject* m_eventSource = nullptr;
};
