#pragma once

#include <QtCore/QScopedPointer>

#include "trigger.h"

class HotkeyTriggerPrivate;

class HotkeyTrigger : public Trigger {
    Q_OBJECT
public:
    HotkeyTrigger(QObject* parent = nullptr);
    virtual ~HotkeyTrigger();

private:
    QScopedPointer<HotkeyTriggerPrivate> d;
};