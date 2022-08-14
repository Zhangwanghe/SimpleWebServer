#include "Reactor.h"
#include "../Acceptor/Acceptor.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<string.h>
#include <cassert>
#include <errno.h>
#include <stdio.h>
#include <iostream>
using namespace std;

void Reactor::init(int port, int threadCount) {
    m_port = port;
    m_threadPool = make_shared<ThreadPool>(threadCount);
}

void Reactor::startup() {
    init_listen();
    init_epoll();    
    add_epoll(m_listenfd);
}

void Reactor::init_listen() {
    m_listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(m_listenfd >= 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(m_port);

    int ret = 0;
    ret = bind(m_listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    assert(ret >= 0);
    ret = listen(m_listenfd, 10);
    assert(ret >= 0);
}

void Reactor::init_epoll() {
    m_epollfd = epoll_create(1);
    assert(m_epollfd >= 0);
}

void Reactor::add_epoll(int fd) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLRDHUP;
    epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &event);
}

void Reactor::eventloop() {
    while (true) {
        int num = epoll_wait(m_epollfd, m_events, MAX_EVENT_NUM, -1);
        if (num < 0 && errno != EINTR)
        {
            cout << "epoll failure" << endl;
            break;
        }

        for (int i = 0; i < num; ++i) {
            dispatch(m_events[i]);
        }
    }
}

void Reactor::dispatch(const epoll_event& event) {
    int fd = event.data.fd;
    if (fd == m_listenfd) {
        Acceptor acceptor;
        auto opt = acceptor.accept_connect(m_listenfd);
        if (opt) {
            auto p = opt.value();
            m_fd2Handler[p.first] = p.second;
            add_epoll(p.first);
        }        
    } else if (m_fd2Handler.count(fd) > 0) {
        auto handler = m_fd2Handler[fd];
        if (event.events & EPOLLIN) {
            
            handler->read(m_threadPool);
        }

        if (event.events & EPOLLOUT) {
            handler->write();
        }
    }
}

void Reactor::release() {

}