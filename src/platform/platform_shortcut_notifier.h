#pragma once

#include <QtCore/QObject>

class PlatformShortcutNotifier : public QObject {
    Q_OBJECT
public:
    PlatformShortcutNotifier(QObject* parent) : QObject(parent) {}

signals:
    void activated();
};
