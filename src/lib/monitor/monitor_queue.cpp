#include "monitor_queue.h"

void MonitorQueue::addAction(const MonitorAction& action) {
    QMutexLocker locker(&m_mutex);
    m_actions[{ action.type, action.property }] = action.value;
}

bool MonitorQueue::popAction(MonitorAction* action) {
    QMutexLocker locker(&m_mutex);
    if (m_actions.empty())
        return false;

    auto pos = m_actions.begin();

    action->type = pos->first.first;
    action->property = pos->first.second;
    action->value = pos->second;

    m_actions.erase(pos);
    return true;
}
