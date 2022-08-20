#ifndef __HandlerET_h__
#define __HandlerET_h__
#include "Handler.h"
#include "../ThreadPool/Runnable.h"

enum HandlerStatus {
    HandlerRead,
    HandlerWrite
};

class HandlerET : public Runnable, public Handler {
    HandlerStatus m_status;
    std::shared_ptr<IThreadPool> m_threadPool;

public:
    HandlerET(int fd, const std::shared_ptr<Epoll>& epoll);

    void setStatus(HandlerStatus status);

    virtual void run();

    virtual void clear();
};

#endif