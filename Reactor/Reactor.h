#ifndef __Reactor_h__
#define __Reactor_h__
#include "../Handler/Handler.h"
#include "../ThreadPool/IThreadPool.h"
#include "../Acceptor/Acceptor.h"
#include "../public/Epoll.h"
#include <sys/epoll.h>
#include <unordered_map>
#include <memory>

class Reactor {
    int m_port;
    int m_listenfd;
    Acceptor m_acceptor;
    std::shared_ptr<Epoll> m_epoll;

    std::unordered_map<int, std::shared_ptr<Handler>> m_fd2Handler;
    std::shared_ptr<IThreadPool> m_threadPool;

    int m_eventfd;

    bool m_exit = false;

    void initListen();
    
    void dispatch(const epoll_event& event);

    void release();

public:
    void init(int port, std::shared_ptr<IThreadPool> threadPool);
    
    void startup();

    void eventloop();

    void sigHandler(int sig);
};

#endif