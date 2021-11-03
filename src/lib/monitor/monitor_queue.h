#pragma once

#include <unordered_map>

#include <QtCore/QMutex>

#include <util/hash.h>

#include "monitor_action.h"

class MonitorQueue {
public:
    void addAction(const MonitorAction& action);
    bool popAction(MonitorAction* action);
    void clear();

private:
    QMutex m_mutex;
    std::unordered_map<std::pair<MonitorAction::Type, PlatformMonitorProperty>, float> m_actions;
};
