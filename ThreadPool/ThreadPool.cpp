#include "ThreadPool.h"
#include "Worker.h"

ThreadPool::ThreadPool(int threadNum)
{
    this->m_threadNum = threadNum;
}

void ThreadPool::schedule(const TaskType& task) {
    m_taskQueue.push(task);

    if (m_workerSet.size() < m_threadNum) {
        addWorker();
    }
}

void ThreadPool::addWorker() {
    m_workerSet.insert(new Worker);
}