#include "ThreadPool.h"
#include "Worker.h"
using namespace std;

ThreadPool::ThreadPool(int threadNum)
{
    this->m_threadNum = threadNum;
}

void ThreadPool::schedule(const TaskType& task) {
    std::lock_guard<std::recursive_mutex> guard(m_mutex);
    m_taskQueue.push(task);

    if (m_workers.size() < m_threadNum) {
        addWorker();
    }
}

void ThreadPool::addWorker() {
    std::lock_guard<std::recursive_mutex> guard(m_mutex);
    if (m_workers.size() < m_threadNum) {
        auto worker = new Worker;
        thread t(&ThreadPool::executeWorker, this, worker);
        t.detach();   
    } 
}

void ThreadPool::removeWorker(Worker* worker) {
    std::lock_guard<std::recursive_mutex> guard(m_mutex);   
}

void ThreadPool::executeWorker(Worker* worker) {
    worker->run();
}
