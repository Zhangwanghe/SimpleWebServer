#include <functional>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <thread>

typedef std::function<void()> TaskType;
class Worker;
class ThreadPool {
    int m_threadNum;
    std::unordered_map<std::thread::id, std::thread> m_workers;
    std::queue<TaskType> m_taskQueue; // no limit at now

    std::recursive_mutex  m_mutex;

    void addWorker();

    void removeWorker(Worker* worker);

    void executeWorker(Worker* worker);

public:
    ThreadPool(int threadNum);

    void schedule(const TaskType& task);

    //void waitAll();
};
