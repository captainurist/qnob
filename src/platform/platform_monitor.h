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
     * \returns                         System-wide unique identifier for this monitor.
     */
    virtual QString deviceId() const = 0;

    /**
     * \returns                         Name of this monitor. On windows, this is the name as shown in device manager.
     */
    virtual QString name() const = 0;

    /**
     * Reads monitor capabilities. This function might take over a second to complete.
     *
     * \returns                         Supported properties for this monitor.
     */
    virtual Properties capabilities() const = 0;

    /**
     * Reads a monitor property. This function might take over 50ms to complete.
     *
     * Errors are reported into debug log.
     *
     * \param property                  Property to read.
     * \returns                         Current value for the property, or NAN in case of an error.
     */
    virtual float property(Property property) const = 0;

    /**
     * Updates a monitor properly. This function might take over 100ms to complete.
     *
     * Errors are reported into debug log.
     *
     * \param property                  Property to write.
     * \param value                     New value for the property.
     */
    virtual void setProperty(Property property, float value) = 0;
};

QB_DELARE_OPERATORS_FOR_BITSET_FLAGS(PlatformMonitor::Properties)
