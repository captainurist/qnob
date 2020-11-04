#pragma once

#include <QtCore/QObject>

class Knob : public QObject {
    Q_OBJECT
public:
    static constexpr int MinValue = 0;
    static constexpr int MaxValue = 1000000;

    Knob(QObject* parent = nullptr) : QObject(parent) {}

    virtual QByteArray id() const = 0;

    virtual int value() const = 0;
    virtual void setValue(int value) = 0;

    virtual bool enabled() const = 0;
    virtual void setEnabled(bool value) = 0;

signals:
    void valueChanged(int value);
    void enabledChanged(bool value);
};

