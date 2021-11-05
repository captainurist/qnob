#pragma once

#include <vector>
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
     * \param worker                    Worker object.
     */
    void kill(QObject* worker);

    /**
     * Blocks until all workers are destroyed. Note that if `kill` wasn't called for some of the workers, this method
     * will block indefinitely.
     */
    void waitForDone();

    /**
     * \returns                         Global worker pool singleton.
     */
    static WorkerPool* globalInstance();

private:
    QThread* reuseThread();
    void releaseWorker(QObject* worker);

private:
    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    std::unordered_map<QObject*, QThread*> m_threadByWorker;
    std::vector<QThread*> m_freeThreads;
};
