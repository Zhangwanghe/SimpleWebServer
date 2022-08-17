#include "ThreadPool.h"
#include <iostream>
using namespace std;

ThreadPool::ThreadPool(int threadNum)
{
    this->m_threadNum = threadNum;
}

void ThreadPool::schedule(shared_ptr<Runnable>& task) {
    lock_guard<recursive_mutex> guard(m_mutex);
    m_taskQueue.push(task);

    if (m_workers.size() < m_threadNum) {
        addWorker();
    }
}

void ThreadPool::addWorker() {
    lock_guard<recursive_mutex> guard(m_mutex);
    if (m_workers.size() < m_threadNum) {
        auto worker = make_shared<Worker>();
        thread t(&ThreadPool::executeWorker, this, worker);
        m_workers.insert(t.get_id());
        t.detach();   
    } 
}

void ThreadPool::removeWorker(thread::id id) {
    lock_guard<recursive_mutex> guard(m_mutex);   
    m_workers.erase(id);
}

void ThreadPool::executeWorker(const shared_ptr<Worker>& worker) {
    worker->run(shared_from_this());
}

optional<shared_ptr<Runnable>> ThreadPool::getTask() {
    lock_guard<recursive_mutex> guard(m_mutex);   
    if (m_taskQueue.empty()) {
        return nullopt;
    }

    auto ret = m_taskQueue.back();
    m_taskQueue.pop();
    return ret;
}
