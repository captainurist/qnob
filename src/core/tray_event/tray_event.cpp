#include "tray_event.h"

#include <QtGui/QWheelEvent>
#include <QtWidgets/QSystemTrayIcon>

#include <platform/platform.h>

static QKeyCombination keyFromEvent(QWheelEvent* event) {
    Qt::Key key;

    if (event->angleDelta().y() > 0) {
        key = Qt::Key_WheelUp;
    } else if (event->angleDelta().y() < 0) {
        key = Qt::Key_WheelDown;
    } else if (event->angleDelta().x() > 0) {
        key = Qt::Key_WheelRight;
    } else if (event->angleDelta().x() < 0) {
        key = Qt::Key_WheelLeft;
    } else {
        key = Qt::Key_unknown;
    }

    return key | event->modifiers();
}

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
    if (event->type() == QEvent::Wheel && m_key == keyFromEvent(static_cast<QWheelEvent*>(event)))
        emit triggered();

    return false;
}
