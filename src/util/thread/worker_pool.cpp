#include "worker_pool.h"

#include <cassert>

#include <QtCore/QThread>

Q_GLOBAL_STATIC(WorkerPool, g_workerPool)

WorkerPool::WorkerPool() {}

WorkerPool::~WorkerPool() {
    /* Note that we need a mutex here because releaseWorker can be simultaneously called from another thread. */
    QMutexLocker locker(&m_mutex);
    waitForDoneLocked();
    assert(m_threadByKilledWorker.empty() && m_threadByWorker.empty());

    for (auto&& thread : m_freeThreads)
        thread->exit();
    for (auto&& thread : m_freeThreads)
        thread->wait();
    m_freeThreads.clear(); /* Destroys all thread objects. */
}

void WorkerPool::run(std::unique_ptr<QObject> worker) {
    std::unique_ptr<QThread> thread = reuseThread();

    if (!thread) {
        thread = std::make_unique<QThread>();
        thread->setObjectName(lit("WorkerThread"));
        thread->moveToThread(this->thread());
        thread->setParent(this);
        thread->start();
    }

    connect(worker.get(), &QObject::destroyed, this, &WorkerPool::releaseWorker, Qt::DirectConnection);
    worker->moveToThread(thread.get());

    QMutexLocker locker(&m_mutex);
    m_threadByWorker.emplace(worker.release(), std::move(thread));
}

void WorkerPool::kill(QObject* worker) {
    QMutexLocker locker(&m_mutex);
    killLocked(worker);
}

void WorkerPool::killLocked(QObject* worker) {
    assert(!m_mutex.try_lock()); /* Must be called under lock. */
    assert(m_threadByWorker.contains(worker));

    worker->deleteLater();
    m_threadByKilledWorker.insert(m_threadByWorker.extract(worker));
}

void WorkerPool::waitForDone() {
    QMutexLocker locker(&m_mutex);
    waitForDoneLocked();
}

void WorkerPool::waitForDoneLocked() {
    assert(!m_mutex.try_lock()); /* Must be called under lock. */

    while (!m_threadByWorker.empty())
        killLocked(m_threadByWorker.begin()->first);

    while (!m_threadByKilledWorker.empty())
        m_waitCondition.wait(&m_mutex);
}

WorkerPool* WorkerPool::globalInstance() {
    return g_workerPool();
}

std::unique_ptr<QThread> WorkerPool::reuseThread() {
    QMutexLocker locker(&m_mutex);

    if (m_freeThreads.empty())
        return nullptr;

    std::unique_ptr<QThread> result = std::move(m_freeThreads.back());
    m_freeThreads.pop_back();
    return result;
}

void WorkerPool::releaseWorker(QObject* worker) {
    QMutexLocker locker(&m_mutex);

    assert(m_threadByKilledWorker.contains(worker));

    m_freeThreads.emplace_back(std::move(m_threadByKilledWorker.extract(worker).mapped()));
    m_waitCondition.wakeOne();
}
