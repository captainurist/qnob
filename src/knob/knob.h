#pragma once

#include <QtCore/QObject>

class Knob : public QObject {
    Q_OBJECT
public:
    Knob(QObject* parent = nullptr) : QObject(parent) {}

    virtual QByteArray id() const = 0;

    virtual double value() const = 0;
    virtual void setValue(double value) = 0;

    virtual bool enabled() const = 0;
    virtual void setEnabled(bool value) = 0;

signals:
    void valueChanged(double value);
    void enabledChanged(bool value);
};

