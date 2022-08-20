#ifndef __ThreadPoolStatic_h__
#define __ThreadPoolStatic_h__
#include "../IThreadPool.h"
#include "WorkerStatic.h"
#include "../Runnable.h"
#include <functional>
#include <unordered_set>
#include <queue>
#include <mutex>
#include <thread>
#include <optional>
#include <memory>

class ThreadPoolStatic : public std::enable_shared_from_this<ThreadPoolStatic>, public IThreadPool {
    int m_threadNum = 0;
    int m_workerNum = 0;
    
    std::queue<std::shared_ptr<Runnable>> m_taskQueue; // no limit at now

    std::recursive_mutex  m_mutex;

    void addWorker();

    void executeWorker(const std::shared_ptr<WorkerStatic>& worker);

    std::optional<std::shared_ptr<Runnable>> getTask();

    friend class WorkerStatic;

public:
    ThreadPoolStatic(int threadNum);

    void schedule(const std::shared_ptr<Runnable>& task);

    //void waitAll();
};

#endif