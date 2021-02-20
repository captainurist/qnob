#include "win_ddc_monitor_queue.h"

void WinDdcMonitorQueue::addAction(WinDdcMonitorAction action) {
    QMutexLocker locker(&m_mutex);
    m_actions[{ action.property, action.type }] = action.value;
}

bool WinDdcMonitorQueue::popAction(WinDdcMonitorAction* action) {
    QMutexLocker locker(&m_mutex);
    if (m_actions.empty())
        return false;

    auto pos = m_actions.begin();

    action->property = pos->first.first;
    action->type = pos->first.second;
    action->value = pos->second;

    m_actions.erase(pos);
    return true;
}
