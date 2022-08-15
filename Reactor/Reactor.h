#ifndef __Reactor_h__
#define __Reactor_h__
#include "../Handler/Handler.h"
#include "../ThreadPool/ThreadPool.h"
#include "../public/Epoll.h"
#include <sys/epoll.h>
#include <unordered_map>
#include <memory>

class Reactor {
    int m_port;
    int m_listenfd;
    Epoll m_epoll;

    std::unordered_map<int, std::shared_ptr<Handler>> m_fd2Handler;
    std::shared_ptr<ThreadPool> m_threadPool;

    void init_listen();

    void dispatch(const epoll_event& event);

public:
    void init(int port, int threadCount);
    
    void startup();

    void eventloop();

    void release();
};

#endif