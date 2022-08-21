#ifndef __HandlerET_h__
#define __HandlerET_h__
#include "Handler.h"
#include "../ThreadPool/Runnable.h"

class HandlerET : public Runnable, public Handler {
public:
    HandlerET(int fd, const std::shared_ptr<Epoll>& epoll);
    
    virtual bool read(const std::shared_ptr<IThreadPool>& threadPool);
    
    virtual void run();

    virtual void clear();
};

#endif