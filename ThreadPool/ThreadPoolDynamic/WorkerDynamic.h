#ifndef __WorkerDynamic_h__
#define __WorkerDynamic_h__
#include <memory>

class ThreadPoolDynamic;
class WorkerDynamic {
public:
    void run(std::shared_ptr<ThreadPoolDynamic> pool);
};

#endif