#include "Reactor.h"
#include "../Acceptor/Acceptor.h"

#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <cassert>
#include <errno.h>
#include <stdio.h>
#include <iostream>
using namespace std;

void Reactor::init(int port, int threadCount) {
    m_port = port;
    m_threadPool = make_shared<ThreadPool>(threadCount);
    m_epoll = make_shared<Epoll>();
}

void Reactor::startup() {
    init_listen();
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

    m_epoll->addfd(m_listenfd);
}

void Reactor::eventloop() {
    while (true) {
        auto opt = m_epoll->waitEvents();
        if (!opt) {
            break;
        }

        for (int i = 0; i < opt->first; ++i) {
            dispatch(opt->second[i]);
        }
    }

    release();
}

void Reactor::dispatch(const epoll_event& event) {
    int fd = event.data.fd;
    if (fd == m_listenfd) {
        Acceptor acceptor;
        auto opt = acceptor.accept_connect(m_listenfd, m_epoll);
        if (opt) {
            auto p = opt.value();
            m_fd2Handler[p.first] = p.second;
            m_epoll->addfd(p.first);
        }        
    } else if (m_fd2Handler.count(fd) > 0) {
        //cout << event.events << endl;
        bool succeed = true;

        auto handler = m_fd2Handler[fd];
        if (event.events & EPOLLIN) {
            // todo ||
            succeed = handler->read(m_threadPool);
        }

        if (event.events & EPOLLOUT) {
            succeed = handler->write();

            //m_epoll->addEvent(fd, EPOLLIN);
            // todo use return value to determine
            m_fd2Handler.erase(fd);
        }

        if (!succeed || (event.events & (EPOLLHUP | EPOLLERR))) {
            m_fd2Handler.erase(fd);
            m_epoll->removefd(fd);
        }
    }
}

void Reactor::release() {
    for (auto& [fd, handler] : m_fd2Handler) {
        m_epoll->removefd(fd);
    }
}