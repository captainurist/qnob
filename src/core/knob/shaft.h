#pragma once

#include <QtCore/QObject>

/**
 * Shaft is what you attach a knob on.
 */
class Shaft : public QObject {
    Q_OBJECT
public:
    virtual double value() const = 0;
    virtual void setValue(double value) = 0;

    virtual bool enabled() const = 0;
    virtual void setEnabled(bool value) = 0;

signals:
    void notificationReceived(); // TODO: changedExternally()
};

