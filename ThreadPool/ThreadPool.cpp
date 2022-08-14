#include "ThreadPool.h"
#include "Worker.h"
using namespace std;

ThreadPool::ThreadPool(int threadNum)
{
    this->m_threadNum = threadNum;
}

void ThreadPool::schedule(Runnable* task) {
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
        //todo m_workers[t.get_id()] = t;
        t.detach();   
    } 
}

void ThreadPool::removeWorker(thread::id id) {
    std::lock_guard<std::recursive_mutex> guard(m_mutex);   
    //todo 
}

void ThreadPool::executeWorker(Worker* worker) {
    worker->run(this);
}

std::optional<Runnable*> ThreadPool::getTask() {
    std::lock_guard<std::recursive_mutex> guard(m_mutex);   
    if (m_taskQueue.empty()) {
        return nullopt;
    }

    auto ret = m_taskQueue.back();
    m_taskQueue.pop();
    return ret;
}
