#pragma once

#include <QtCore/QObject>

#include <util/thread/package.h>

#include "monitor_list.h"

class PlatformMonitorManager;

class MonitorEnumerator : public QObject {
    Q_OBJECT
public:
    MonitorEnumerator(QObject* parent);
    virtual ~MonitorEnumerator();

    Q_SLOT void enumerate();

signals:
    void finished(Package<MonitorList> result);

private:
    void handleMonitorInitializated();

private:
    MonitorList m_result;
    size_t m_pendingInitializations = 0;
};
