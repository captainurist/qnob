#pragma once

#include <QtCore/QObject>

/**
 * Settings backend that does all the heavy lifting.
 *
 * Implementations are encouraged to just pass all calls to the underlying API and don't cache anything.
 */
class SettingBackend : public QObject {
    Q_OBJECT
public:
    SettingBackend(QObject* parent) : QObject(parent) {}

    /**
     * \returns                         Current value, in [0, 1], or NAN if this backend is not yet initialized.
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
     * Enables / disables a setting. Not all backends support this function, so it might have no effect.
     *
     * param value                      New enabled state.
     */
    virtual void setEnabled(bool value) = 0;

    /**
     * \returns                         Whether this backend has been initialized.
     */
    virtual bool isInitialized() const = 0;

signals:
    /**
     * Some backends might provide notifications for state changes that were made externally, i.e. not through this
     * interface.
     */
    void changedExternally();

    /**
     * Some backends might take time to initialize. Use this signal to get a notification when the backend
     * finishes initialization.
     *
     * Note that this API is not perfect and there exists an inherent race condition here - backend might get
     * initialized right after construction, but before a connection to this signal is created.
     */
    void initialized();
};

