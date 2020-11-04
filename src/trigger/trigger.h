#pragma once

#include <QtCore/QObject>

class Trigger : public QObject {
    Q_OBJECT
public:
    Trigger(QObject* parent = nullptr) : QObject(parent) {}

signals:
    void triggered();
};
