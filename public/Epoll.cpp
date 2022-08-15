#include "Epoll.h"
#include <iostream>
#include <cassert>
using namespace std;

Epoll::Epoll() {
    m_epollfd = epoll_create(1);
    assert(m_epollfd >= 0);
}

void Epoll::addfd(int fd) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &event);
}

void Epoll::addEvent(int fd, int ev) {
    epoll_event event;
    event.data.fd = fd;
    event.events = ev;
    epoll_ctl(m_epollfd, EPOLL_CTL_MOD, fd, &event);
}

void Epoll::removefd(int fd) {
    epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, 0);
}

optional<pair<int, epoll_event*>> Epoll::waitEvents() {
    int num = epoll_wait(m_epollfd, m_events, MAX_EVENT_NUM, -1);
    if (num < 0 && errno != EINTR)
    {
        cout << "epoll failure" << endl;
        return nullopt;
    }

    return make_optional(make_pair(num, &m_events[0]));
}