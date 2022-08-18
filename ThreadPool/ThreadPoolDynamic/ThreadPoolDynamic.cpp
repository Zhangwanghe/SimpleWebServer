#include "ThreadPoolDynamic.h"
#include <iostream>
using namespace std;

ThreadPoolDynamic::ThreadPoolDynamic(int threadNum)
{
    this->m_threadNum = threadNum;
}

void ThreadPoolDynamic::schedule(shared_ptr<Runnable>& task) {
    lock_guard<recursive_mutex> guard(m_mutex);
    m_taskQueue.push(task);

    if (m_workers.size() < m_threadNum) {
        addWorker();
    }
}

void ThreadPoolDynamic::addWorker() {
    lock_guard<recursive_mutex> guard(m_mutex);
    if (m_workers.size() < m_threadNum) {
        auto worker = make_shared<WorkerDynamic>();
        thread t(&ThreadPoolDynamic::executeWorker, this, worker);
        m_workers.insert(t.get_id());
        t.detach();   
    } 
}

void ThreadPoolDynamic::removeWorker(thread::id id) {
    lock_guard<recursive_mutex> guard(m_mutex);   
    m_workers.erase(id);
}

void ThreadPoolDynamic::executeWorker(const shared_ptr<WorkerDynamic>& worker) {
    worker->run(shared_from_this());
}

optional<shared_ptr<Runnable>> ThreadPoolDynamic::getTask() {
    lock_guard<recursive_mutex> guard(m_mutex);   
    if (m_taskQueue.empty()) {
        return nullopt;
    }

    auto ret = m_taskQueue.front();
    m_taskQueue.pop();
    return ret;
}
