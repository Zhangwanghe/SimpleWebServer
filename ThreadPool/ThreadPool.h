#ifndef __ThreadPool_h__
#define __ThreadPool_h__

#include <functional>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <thread>
#include <optional>

class Worker;
class Runnable;
class ThreadPool {
    int m_threadNum;
    std::unordered_map<std::thread::id, std::thread> m_workers;
    std::queue<Runnable*> m_taskQueue; // no limit at now

    std::recursive_mutex  m_mutex;

    void addWorker();

    void executeWorker(Worker* worker);

    std::optional<Runnable*> getTask();

    void removeWorker(std::thread::id id);

    friend class Worker;

public:
    ThreadPool(int threadNum);

    void schedule(Runnable* task);

    //void waitAll();
};

#endif