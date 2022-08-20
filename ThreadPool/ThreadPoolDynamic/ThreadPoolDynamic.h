#ifndef __ThreadPoolDynamic_h__
#define __ThreadPoolDynamic_h__
#include "../IThreadPool.h"
#include "WorkerDynamic.h"
#include "../Runnable.h"
#include <functional>
#include <unordered_set>
#include <queue>
#include <mutex>
#include <thread>
#include <optional>
#include <memory>

class ThreadPoolDynamic : public std::enable_shared_from_this<ThreadPoolDynamic>, public IThreadPool {
    int m_threadNum;
    std::unordered_set<std::thread::id> m_workers;
    std::queue<std::shared_ptr<Runnable>> m_taskQueue; // no limit at now

    std::recursive_mutex  m_mutex;

    void addWorker();

    void executeWorker(const std::shared_ptr<WorkerDynamic>& WorkerDynamic);

    std::optional<std::shared_ptr<Runnable>> getTask();

    void removeWorker(std::thread::id id);

    friend class WorkerDynamic;

public:
    ThreadPoolDynamic(int threadNum);

    void schedule(const std::shared_ptr<Runnable>& task);

    //void waitAll();
};

#endif