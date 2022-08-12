#include <sys/epoll.h>
#include <unordered_map>

class Handler;
class Reactor {
    int m_port;
    int m_listenfd;
    int m_epollfd;

    static const int MAX_EVENT_NUM = 10000;
    epoll_event m_events[MAX_EVENT_NUM];

    std::unordered_map<int, Handler*> m_fd2Handler;

    void init_listen();

    void init_epoll();

    void add_epoll(int fd);

    void dispatch(const epoll_event& event);

public:
    void init(int port);
    
    void startup();

    void eventloop();

    void release();
};