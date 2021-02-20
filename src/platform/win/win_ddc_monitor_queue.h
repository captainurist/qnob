#pragma once

#include <unordered_map>

#include <QtCore/QMutex>

#include <util/hash.h>

#include "win_ddc_monitor_action.h"

class WinDdcMonitorQueue {
public:
    void addAction(WinDdcMonitorAction action);
    bool popAction(WinDdcMonitorAction* action);

private:
    QMutex m_mutex;
    std::unordered_map<std::pair<PlatformMonitor::Property, WinDdcMonitorAction::Type>, DWORD> m_actions;
};
