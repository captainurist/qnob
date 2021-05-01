#include "tray_event.h"

#include <QtGui/QWheelEvent>
#include <QtWidgets/QSystemTrayIcon>

#include <platform/platform.h>

TrayEvent::TrayEvent(const QString& id) :
    Entity(id)
{}

TrayEvent::~TrayEvent() {}

void TrayEvent::setEventSource(QObject* eventSource) {
    if (m_eventSource)
        m_eventSource->removeEventFilter(this);

    m_eventSource = eventSource;

    if (m_eventSource)
        m_eventSource->installEventFilter(this);
}

QObject* TrayEvent::eventSource() const {
    return m_eventSource;
}

void TrayEvent::setKey(QKeyCombination key) {
    m_key = key;
}

QKeyCombination TrayEvent::key() const {
    return m_key;
}

bool TrayEvent::eventFilter(QObject* /*watched*/, QEvent* event) {
    if (event->type() == QEvent::Wheel)
        return wheelEvent(static_cast<QWheelEvent*>(event));

    return false;
}

bool TrayEvent::wheelEvent(QWheelEvent* event) {
    int delta = event->angleDelta().y();

    if ((delta > 0 && m_key.key() == Qt::Key_WheelUp) || (delta < 0 && m_key.key() == Qt::Key_WheelDown)) {
        emit triggered();
        return true;
    }

    return false;
}
