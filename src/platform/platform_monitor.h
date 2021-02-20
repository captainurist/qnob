#pragma once

#include <QtCore/QObject>

class PlatformMonitor: public QObject {
    Q_OBJECT
public:
    enum Property {
        BrightnessProperty,
        ContrastProperty,
        PropertyCount
    };

    /**
     * \returns                         Name of this monitor.
     */
    virtual QString name() const = 0;

    /**
     * \param property                  Property to check.
     * \returns                         Whether the provided property is supported.
     */
    virtual bool supported(Property property) const = 0;

    /**
     * \param property                  Property to read.
     * \returns                         Current value for the property.
     */
    virtual float property(Property property) const = 0;

    /**
     * Modifier for properly values. This function returns immediately, but actual updates might happen on a different
     * thread, and might take some time to finish. This is why reading a property right after this call won't return
     * the new value.
     *
     * You can subscribe to `propertyChanged` if you want to receive a notification when the underlying API has
     * finished. Note however that implementation might merge several consecutive calls to `setProperty`, and thus
     * you might get only one notification for several calls.
     *
     * \param property                  Property to write.
     * \param value                     New value for the property.
     */
    virtual void setProperty(Property property, float value) = 0;

signals:
    void propertyChanged(Property property);
};

Q_DECLARE_METATYPE(PlatformMonitor::Property)
