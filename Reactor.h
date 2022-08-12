#include <sys/epoll.h>

class Reactor {
    int m_port;
    int m_listenfd;
    int m_epollfd;

    static const int MAX_EVENT_NUM = 10000;
    epoll_event m_events[MAX_EVENT_NUM];

    void init_listen();

    void init_epoll();

    void add_epoll(int fd);

public:
    void init(int port);
    
    void startup();

    void eventloop();
};