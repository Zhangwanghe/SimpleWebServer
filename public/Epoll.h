#ifndef __Epoll_h__
#define __Epoll_h__
#include <optional>
#include <sys/epoll.h>

enum EpollMode {
    EPOLL_LT,
    EPOLL_ET
};
class Epoll {
    int m_epollfd;
    int m_eventMask = EPOLLHUP | EPOLLRDHUP;

    static const int MAX_EVENT_NUM = 10000;
    epoll_event m_events[MAX_EVENT_NUM];

    int setnonblocking(int fd);
    
public:
    Epoll(EpollMode mode = EPOLL_LT);

    void setMode(EpollMode mode);

    void addfd(int fd);

    void addEvent(int fd, int ev);

    void removefd(int fd);

    std::optional<std::pair<int, epoll_event*>> waitEvents();

    void release();
};

#endif