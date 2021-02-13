#pragma once

#include <QtCore/QObject>

class PlatformBrightnessControl : public QObject {
    Q_OBJECT
public:
    virtual ~PlatformBrightnessControl() {}

    virtual float brightness() const = 0;
    virtual void setBrightness(float brightness) = 0;

    virtual float contrast() const = 0;
    virtual void setContrast(float contrast) = 0;

signals:
    void notificationReceived();
};