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
     * \returns                         Whether this control is visible.
     */
    virtual bool isVisible() const = 0;

    /**
     * \param visible                   New visibility for this control.
     */
    virtual void setVisible(bool visible) = 0;
};
