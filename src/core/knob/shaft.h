#pragma once

#include <QtCore/QObject>

/**
 * Shaft is what you attach a knob on.
 */
class Shaft : public QObject {
    Q_OBJECT
public:
    /**
     * \returns                         Current value, in [0, 1], or NAN if this shaft is broken or not yet initialized.
     */
    virtual double value() const = 0;

    /**
     * \param value                     New value. Must be in [0, 1].
     */
    virtual void setValue(double value) = 0;

    /**
     * \returns                         Enabled state.
     */
    virtual bool isEnabled() const = 0;

    /**
     * Enables / disables a shaft. Not all shafts support this function, so it might have no effect.
     *
     * param value                      New enabled state.
     */
    virtual void setEnabled(bool value) = 0;

signals:
    /**
     * Some shafts might provide notifications for state changes that were made externally, i.e. not through `Shaft`
     * interface.
     */
    void changedExternally();
};

