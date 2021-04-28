#pragma once

#include <QtCore/QObject>

class PlatformMetrics : public QObject {
    Q_OBJECT
public:
    virtual QSize trayIconSize() const = 0;

signals:
    void trayIconSizeChanged();
};
