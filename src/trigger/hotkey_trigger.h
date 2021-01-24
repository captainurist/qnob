#pragma once

#include <QtCore/QScopedPointer>

#include "trigger.h"

class HotkeyTrigger : public Trigger {
    Q_OBJECT
public:
    HotkeyTrigger(QObject* parent = nullptr);
    virtual ~HotkeyTrigger();
};