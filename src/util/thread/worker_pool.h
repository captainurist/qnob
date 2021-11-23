#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>

class QThread;

/**
 * Worker pool is an abstraction akin to a thread pool, but for a different kind of workloads.
 *
 * Compare:
 * - Thread pool lets you execute runnables, which essentially are short-lived tasks that are executed on another thread.
 * - Worker pool lets you assign a `QObject` to a long-lived thread, that you can then interact with through Qt events
 *   or signal / slot connections. As the worker will live in another thread, this is where the events / slots will be
 *   executed.
 *
 * It is possible to just create `QThread`s for such use cases, but this has several drawbacks:
 * - Object lifetimes need to be carefully managed.
 * - There's an overhead of creating / destroying system threads when using short-lived workers.
 *
 * This is essentially what worker pool provides - an easy to use abstraction that manages object lifetimes for the user.
 *
 * Note that an alternative design would've been a task queue with tasks executed in a thread pool, but this presents
 * its own set of challenges as it doesn't map that well to the abstractions offered by Qt. E.g. using signals & slots
 * for task scheduling is going to be challenging - will have to roll out custom `connect` method that would create a
 * task instead of sending a `QMetaCallEvent`. Doable, will save some threads, but totally not worth it.
 */
class WorkerPool : public QObject {
    Q_OBJECT
public:
    WorkerPool();
    virtual ~WorkerPool();

    /**
     * Allocates a separate thread for the provided worker object, and moves it into that thread.
     *
     * \param worker                    Worker object.
     */
    void run(std::unique_ptr<QObject> worker);

    /**
     * Schedules a release of the provided worker object that was previously passed to `run`.
     *
     * \param worker                    Worker object. Must be previously started in this worker pool with a call to `run`.
     */
    void kill(QObject* worker);

    /**
     * Kills all workers and blocks until they are actually destroyed.
     */
    void waitForDone();

    /**
     * \returns                         Global worker pool singleton.
     */
    static WorkerPool* globalInstance();

private:
    std::unique_ptr<QThread> reuseThread();
    void releaseWorker(QObject* worker);
    void waitForDoneLocked();
    void killLocked(QObject* worker);

private:
    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    std::unordered_map<QObject*, std::unique_ptr<QThread>> m_threadByWorker;
    std::unordered_map<QObject*, std::unique_ptr<QThread>> m_threadByKilledWorker;
    std::vector<std::unique_ptr<QThread>> m_freeThreads;
};
