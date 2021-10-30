#include "monitor_manager.h"

#include <cassert>

#include <QtCore/QPromise>
#include <QtCore/QEventLoop>
#include <QtCore/QThreadPool>

#include <util/debug.h>

#include <platform/platform_monitor_manager.h>

#include "monitor.h"

class MonitorTask : public QRunnable {
public:
    MonitorTask(PlatformMonitorManager* manager, QThread* thread) :
        m_manager(manager),
        m_thread(thread)
    {
        /* Start the promise on creation so that it's always valid. */
        m_promise.start();
    }

    virtual void run() override {
        QEventLoop eventLoop;
        MonitorManager::MonitorList result;
        size_t initializedCount = 0;

        for (auto& platformMonitor : m_manager->enumerateMonitors()) {
            std::unique_ptr<Monitor> monitor(new Monitor(std::move(platformMonitor)));

            QObject::connect(monitor.get(), &Monitor::initialized, &eventLoop, [&] {
                initializedCount++;
                if (m_promise.isCanceled() || initializedCount == result.size())
                    eventLoop.exit();
            });

            result.emplace_back(std::move(monitor));
        }

        eventLoop.exec();

        if (m_promise.isCanceled()) {
            m_promise.finish();
            return;
        }

        xDebug("Enumerated {} monitor(s)", result.size());
        for (size_t i = 0; i < result.size(); i++)
            xDebug("Monitor #{}: {} (deviceId={})", i, result[i]->name(), result[i]->deviceId());

        assert(initializedCount == result.size());
        for (auto& monitor : result)
            monitor->moveToThread(m_thread);
        m_promise.addResult(std::move(result));
        m_promise.finish();
    }

    QFuture<MonitorManager::MonitorList> future() const {
        return m_promise.future();
    }

private:
    QPromise<MonitorManager::MonitorList> m_promise;
    PlatformMonitorManager* m_manager;
    QThread* m_thread;
};

QFuture<MonitorManager::MonitorList> MonitorManager::enumerateMonitors(PlatformMonitorManager* manager) {
    MonitorTask* task = new MonitorTask(manager, QThread::currentThread());
    QFuture<MonitorManager::MonitorList> result = task->future();

    QThreadPool::globalInstance()->start(task);

    return result;
}
