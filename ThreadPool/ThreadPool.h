#include <functional>
#include <unordered_set>
#include <queue>

typedef std::function<void()> TaskType;
class Worker;
class ThreadPool {
    int m_threadNum;
    std::unordered_set<Worker*> m_workerSet;
    std::queue<TaskType> m_taskQueue; // no limit at now

    void addWorker();

public:
    ThreadPool(int threadNum);

    void schedule(const TaskType& task);

    //void waitAll();
};
