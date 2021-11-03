#pragma once

#include <unordered_set>

#include <QtCore/QObject>
#include <QtCore/QFuture>

#include <util/package.h>

#include "monitor_list.h"

class MonitorEnumerator;

class MonitorManager: public QObject {
    Q_OBJECT
public:
    MonitorManager();
    virtual ~MonitorManager();

    const MonitorList& monitors() const;

signals:
    void monitorsChanged();

private:
    void updateMonitors();
    void handleEnumeratorFinished(Package<MonitorList> monitors);

private:
    MonitorList m_monitors;
};
