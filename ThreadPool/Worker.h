#ifndef __Worker_h__
#define __Worker_h__
#include <memory>

class ThreadPool;
class Worker {
public:
    void run(std::shared_ptr<ThreadPool> pool);
};

#endif