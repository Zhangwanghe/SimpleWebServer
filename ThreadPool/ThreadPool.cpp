#include "ThreadPool.h"
using namespace std;

ThreadPool::ThreadPool(int threadNum)
{
    this->m_threadNum = threadNum;
}

void ThreadPool::schedule(shared_ptr<Runnable>& task) {
    std::lock_guard<std::recursive_mutex> guard(m_mutex);
    m_taskQueue.push(task);

    if (m_workers.size() < m_threadNum) {
        addWorker();
    }
}

void ThreadPool::addWorker() {
    std::lock_guard<std::recursive_mutex> guard(m_mutex);
    if (m_workers.size() < m_threadNum) {
        auto worker = make_shared<Worker>();
        thread t(&ThreadPool::executeWorker, this, worker);
        //todo m_workers[t.get_id()] = t;
        t.detach();   
    } 
}

void ThreadPool::removeWorker(thread::id id) {
    std::lock_guard<std::recursive_mutex> guard(m_mutex);   
    //todo 
}

void ThreadPool::executeWorker(const shared_ptr<Worker>& worker) {
    worker->run(shared_from_this());
}

std::optional<shared_ptr<Runnable>> ThreadPool::getTask() {
    std::lock_guard<std::recursive_mutex> guard(m_mutex);   
    if (m_taskQueue.empty()) {
        return nullopt;
    }

    auto ret = m_taskQueue.back();
    m_taskQueue.pop();
    return ret;
}
