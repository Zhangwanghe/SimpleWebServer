#ifndef __Reactor_h__
#define __Reactor_h__
#include "../Handler/Handler.h"
#include "../ThreadPool/ThreadPool.h"
#include <sys/epoll.h>
#include <unordered_map>
#include <memory>

class Reactor {
    int m_port;
    int m_listenfd;
    int m_epollfd;

    static const int MAX_EVENT_NUM = 10000;
    epoll_event m_events[MAX_EVENT_NUM];

    std::unordered_map<int, std::shared_ptr<Handler>> m_fd2Handler;
    std::shared_ptr<ThreadPool> m_threadPool;

    void init_listen();

    void init_epoll();

    void add_epoll(int fd);

    void dispatch(const epoll_event& event);

public:
    void init(int port, int threadCount);
    
    void startup();

    void eventloop();

    void release();
};

#endif