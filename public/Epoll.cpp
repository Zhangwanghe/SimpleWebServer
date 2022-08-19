#include "Epoll.h"
#include <iostream>
#include <cassert>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

// EPOLLHUP for client close
Epoll::Epoll() {
    m_epollfd = epoll_create(1);
    assert(m_epollfd >= 0);
}

void Epoll::addfd(int fd) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP;
    epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

int Epoll::setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void Epoll::addEvent(int fd, int ev) {
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLHUP | EPOLLRDHUP;
    epoll_ctl(m_epollfd, EPOLL_CTL_MOD, fd, &event);
}

void Epoll::removefd(int fd) {
    close(fd);
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

void Epoll::release() {
    close(m_epollfd);
}