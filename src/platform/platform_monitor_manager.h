#pragma once

#include <QtCore/QObject>

#include "platform_monitor.h"

class PlatformMonitorManager : public QObject {
    Q_OBJECT
public:
    PlatformMonitorManager(QObject* parent) : QObject(parent) {}

    /**
     * This function is thread-safe.
     *
     * \returns                         Newly created platform monitors representing all the monitors in the system.
     */
    virtual std::vector<std::unique_ptr<PlatformMonitor>> enumerateMonitors(QObject* parent) = 0;

signals:
    /**
     * This signal is emitted whenever monitor setup changes (e.g. when a new monitor is connected).
     */
    void monitorsChanged();
};
