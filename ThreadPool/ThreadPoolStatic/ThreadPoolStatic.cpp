#include "ThreadPoolStatic.h"
#include <iostream>
using namespace std;

ThreadPoolStatic::ThreadPoolStatic(int threadNum)
{
    m_threadNum = threadNum;
}

void ThreadPoolStatic::schedule(shared_ptr<Runnable>& task) {
    lock_guard<recursive_mutex> guard(m_mutex);
    m_taskQueue.push(task);

    if (m_threadNum > m_workerNum) {
        addWorker();
    }
}

void ThreadPoolStatic::addWorker() {
    ++m_workerNum;
    auto worker = make_shared<WorkerStatic>();
    thread t(&ThreadPoolStatic::executeWorker, this, worker);
    t.detach();  
}

void ThreadPoolStatic::executeWorker(const shared_ptr<WorkerStatic>& worker) {
    worker->run(shared_from_this());
}

optional<shared_ptr<Runnable>> ThreadPoolStatic::getTask() {
    lock_guard<recursive_mutex> guard(m_mutex);   
    if (m_taskQueue.empty()) {
        return nullopt;
    }

    auto ret = m_taskQueue.front();
    m_taskQueue.pop();
    return ret;
}
