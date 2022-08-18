#ifndef __WorkerStatic_h__
#define __WorkerStatic_h__
#include <memory>

class ThreadPoolStatic;
class WorkerStatic {
public:
    void run(std::shared_ptr<ThreadPoolStatic> pool);
};

#endif