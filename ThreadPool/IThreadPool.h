#ifndef __IThreadPool_h__
#define __IThreadPool_h__
#include <memory>

class Runnable;
class IThreadPool {
public:
    virtual void schedule(const std::shared_ptr<Runnable>& task) = 0;
    
    virtual ~IThreadPool() = 0;
};

#endif