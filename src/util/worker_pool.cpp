#include "worker_pool.h"

#include <cassert>
#include <ranges>

#include <QtCore/QThread>

Q_GLOBAL_STATIC(WorkerPool, g_workerPool)

WorkerPool::WorkerPool() {}

WorkerPool::~WorkerPool() {
    for (QObject* worker : std::views::keys(m_threadByWorker))
        kill(worker);

    waitForDone();

    for (QThread* thread : m_freeThreads)
        thread->exit();
    for (QThread* thread : m_freeThreads)
        thread->wait();
    m_freeThreads.clear();

    /* QThreads will be destroyed later as they are this object's children. */
}

void WorkerPool::run(std::unique_ptr<QObject> worker) {
    QThread* thread = reuseThread();

    if (!thread) {
        thread = new QThread();
        thread->setObjectName(lit("WorkerThread"));
        thread->moveToThread(this->thread());
        thread->setParent(this);
        thread->start();
    }

    worker->moveToThread(thread);
    connect(worker.get(), &QObject::destroyed, this, &WorkerPool::releaseWorker, Qt::DirectConnection);

    QMutexLocker locker(&m_mutex);
    m_threadByWorker.emplace(worker.release(), thread);
}

void WorkerPool::kill(QObject* worker) {
    worker->deleteLater();
}

void WorkerPool::waitForDone() {
    QMutexLocker locker(&m_mutex);
    while (!m_threadByWorker.empty())
        m_waitCondition.wait(&m_mutex);
}

WorkerPool* WorkerPool::globalInstance() {
    return g_workerPool();
}

QThread* WorkerPool::reuseThread() {
    QMutexLocker locker(&m_mutex);

    if (m_freeThreads.empty())
        return nullptr;

    QThread* result = m_freeThreads.back();
    m_freeThreads.pop_back();
    return result;
}

void WorkerPool::releaseWorker(QObject* worker) {
    QMutexLocker locker(&m_mutex);

    assert(m_threadByWorker.contains(worker));

    m_freeThreads.push_back(m_threadByWorker[worker]);
    m_threadByWorker.erase(worker);

    m_waitCondition.wakeOne();
}
