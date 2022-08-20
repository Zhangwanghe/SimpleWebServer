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
    std::shared_ptr<Epoll> m_epoll;

    std::unordered_map<int, std::shared_ptr<Handler>> m_fd2Handler;

    int m_eventfd;

    bool m_exit = false;

    void initListen();
    
    void dispatch(const epoll_event& event);

    void release();

protected:
    std::unique_ptr<Acceptor> m_acceptor;
    
    std::shared_ptr<IThreadPool> m_threadPool;

    virtual void initAcceptor();

    virtual void triggerRead(const std::shared_ptr<Handler>& handler);

    virtual int triggerWrite(const std::shared_ptr<Handler>& handler);

public:
    void init(int port, std::shared_ptr<IThreadPool> threadPool);
    
    void startup();

    void eventloop();

    void sigHandler(int sig);
};

#endif