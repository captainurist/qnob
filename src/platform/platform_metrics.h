#pragma once

#include <QtCore/QObject>

class PlatformMetrics : public QObject {
    Q_OBJECT
public:
    PlatformMetrics(QObject* parent) : QObject(parent) {}

    virtual QSize trayIconSize() const = 0;

signals:
    void trayIconSizeChanged();
};
