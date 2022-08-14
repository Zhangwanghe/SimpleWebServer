#ifndef __ThreadPool_h__
#define __ThreadPool_h__
#include "Worker.h"
#include "Runnable.h"
#include <functional>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <thread>
#include <optional>
#include <memory>

class ThreadPool : public std::enable_shared_from_this<ThreadPool> {
    int m_threadNum;
    std::unordered_map<std::thread::id, std::thread> m_workers;
    std::queue<std::shared_ptr<Runnable>> m_taskQueue; // no limit at now

    std::recursive_mutex  m_mutex;

    void addWorker();

    void executeWorker(const std::shared_ptr<Worker>& worker);

    std::optional<std::shared_ptr<Runnable>> getTask();

    void removeWorker(std::thread::id id);

    friend class Worker;

public:
    ThreadPool(int threadNum);

    void schedule(std::shared_ptr<Runnable>& task);

    //void waitAll();
};

#endif