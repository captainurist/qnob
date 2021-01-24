#pragma once

#include <QtCore/QObject>

class PlatformShortcutNotifier : public QObject {
    Q_OBJECT
public:
    PlatformShortcutNotifier(QObject* parent = nullptr) : QObject(parent) {}

signals:
    void activated();
};
