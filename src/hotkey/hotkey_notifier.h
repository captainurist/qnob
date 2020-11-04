#pragma once

#include <QtCore/QObject>

#include "hotkey.h"

class HotkeyNotifier : public QObject {
    Q_OBJECT
public:
    HotkeyNotifier(QObject* parent = nullptr) : QObject(parent) {}

signals:
    void hotkeyPressed();
};
