#pragma once

#include <QtCore/QObject>

#include <util/bitset_flags.h>

/**
 * Abstraction around a monitor.
 *
 * Underlying implementation will likely use DDC/CI calls, which are insanely slow. So it might be a good idea to do
 * all the calls in a separate thread.
 */
class PlatformMonitor: public QObject {
    Q_OBJECT
public:
    enum Property {
        BrightnessProperty,
        ContrastProperty,
        PropertyCount
    };

    QB_DECLARE_BITSET_FLAGS(Properties, Property, PropertyCount);

    /**
     * \returns                         Name of this monitor.
     */
    virtual QString name() const = 0;

    /**
     * \returns                         Supported properties for this monitor.
     */
    virtual Properties capabilities() const = 0;

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
};

QB_DELARE_OPERATORS_FOR_BITSET_FLAGS(PlatformMonitor::Properties)
