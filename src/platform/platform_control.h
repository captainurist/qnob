#pragma once

#include <QtCore/QObject>
#include <QtCore/QRect>

#include "platform_enums.h"

class PlatformControl: public QObject {
    Q_OBJECT
public:
    PlatformControl(QObject* parent) : QObject(parent) {}

    /**
     * \returns                         Current geometry of this control.
     */
    virtual QRect geometry() const = 0;

    /**
     * \returns                         Standard type if this is a standard control.
     */
    virtual PlatformStandardControl standardType() const = 0;
};
