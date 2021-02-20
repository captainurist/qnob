#pragma once

#include <QtCore/QObject>

/**
 * Shaft is what you attach a knob on.
 */
class Shaft : public QObject {
    Q_OBJECT
public:
    /**
     * \returns                         Current value, in [0, 1].
     */
    virtual double value() const = 0;

    /**
     * \param value                     New value. Must be in [0, 1].
     */
    virtual void setValue(double value) = 0;

    /**
     * \returns                         Enabled state.
     */
    virtual bool enabled() const = 0;

    /**
     * param value                      New enabled state.
     */
    virtual void setEnabled(bool value) = 0;

signals:
    void notificationReceived(); // TODO: changedExternally()
};

