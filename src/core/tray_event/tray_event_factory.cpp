#include "tray_event_factory.h"

#include <core/tray_icon/tray_icon.h>
#include <core/tray_icon/standard_tray_icon.h>

#include <lib/metacall/bound_meta_call.h>

#include "mouse_key.h"
#include "tray_event.h"

TrayEventFactory::TrayEventFactory() :
    EntityFactory(lit("tray_event"))
{}

Entity* TrayEventFactory::createEntity(const EntityCreationContext& ctx) {
    MouseKey key = ctx.require<MouseKey>(lit("trigger"));
    Entity* target = ctx.require<Entity*>(lit("target"));
    Entity* source = ctx.require<Entity*>(lit("source"));
    QString action = ctx.require<QString>(lit("action"));
    QVariantList args = ctx.requireOr<QVariantList>(lit("args"), QVariantList());

    QObject* eventSource = nullptr;
    if (TrayIcon* trayIcon = dynamic_cast<TrayIcon*>(source))
        eventSource = trayIcon->icon();
    if (StandardTrayIcon* trayIcon = dynamic_cast<StandardTrayIcon*>(source))
        eventSource = trayIcon->icon();
    assert(eventSource); // TODO

    TrayEvent* result = new TrayEvent(ctx.id());
    result->setKey(key);
    result->setEventSource(eventSource);

    BoundMetaCall call;
    call.bind(target, action.toUtf8(), args);

    QObject::connect(result, &TrayEvent::triggered, target, [=]() mutable {
        call.invoke();
    });
    return result;
}