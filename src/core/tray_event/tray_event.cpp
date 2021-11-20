#include "tray_event.h"

#include <QtGui/QWheelEvent>
#include <QtWidgets/QSystemTrayIcon>

#include <platform/platform.h>

#include <lib/metacall/bound_meta_call.h>

#include <core/entity/entity_config.h>
#include <core/entity/entity_creation_exception.h>
#include <core/tray_icon/tray_icon.h>
#include <core/tray_icon/standard_tray_icon.h>

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

TrayEvent::~TrayEvent() {}

void TrayEvent::loadFromConfig(const EntityConfig& cfg) {
    QKeyCombination key = cfg.require<QKeyCombination>(lit("trigger"));
    Entity* target = cfg.require<Entity*>(lit("target"));
    Entity* source = cfg.require<Entity*>(lit("source"));
    QString action = cfg.require<QString>(lit("action"));
    QVariantList args = cfg.requireOr<QVariantList>(lit("args"), QVariantList());

    if (!isMouseWheel(key.key()))
        xthrow EntityCreationException(cfg.id(), EntityCreationException::tr("Only mouse wheel with optional modifiers is supported as trigger."));

    QObject* eventSource = nullptr;
    if (TrayIcon* trayIcon = dynamic_cast<TrayIcon*>(source))
        eventSource = trayIcon->icon();
    if (StandardTrayIcon* trayIcon = dynamic_cast<StandardTrayIcon*>(source))
        eventSource = trayIcon->icon();
    assert(eventSource); // TODO

    setKey(key);
    setEventSource(eventSource);

    BoundMetaCall call;
    call.bind(target, action.toUtf8(), args);
    QObject::connect(this, &TrayEvent::triggered, target, [=]() mutable { // TODO: not mutable
        call.invoke();
    });
}

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
