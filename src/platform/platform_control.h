#pragma once

#include <QtCore/QObject>
#include <QtCore/QRect>

#include "platform_enums.h"

class PlatformControl: public QObject {
    Q_OBJECT
public:
    PlatformControl(QObject* parent) : QObject(parent) {}

    /**
     * \returns                         Current geometry of this control, in screen coordinates.
     *                                  Might return an empty rect in case of an error.
     */
    virtual QRect geometry() const = 0;

    /**
     * \returns                         Whether this control is minimized.
     */
    virtual bool isMinimized() const = 0;

    /**
     * \param minimized                 New minimized state for this control.
     */
    virtual void setMinimized(bool minimized) = 0;
};
