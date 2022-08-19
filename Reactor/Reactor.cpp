#include "Reactor.h"
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <cassert>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sys/eventfd.h>
using namespace std;

void Reactor::init(int port, std::shared_ptr<IThreadPool> threadPool) {
    m_port = port;
    m_threadPool = threadPool;
    m_epoll = make_shared<Epoll>();
    m_eventfd = eventfd(0, 0);
    m_epoll->addfd(m_eventfd);
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

void Reactor::sigHandler(int sig) {
    m_epoll->release();
    write(m_eventfd, &m_eventfd, sizeof(m_eventfd));
}

void Reactor::eventloop() {
    while (!m_exit) {
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
        auto opt = m_acceptor.accept_connect(m_listenfd, m_epoll);
        if (opt) {
            auto p = opt.value();
            m_fd2Handler[p.first] = p.second;
            m_epoll->addfd(p.first);
        }        
    } else if (fd == m_eventfd) {
        if (event.events & EPOLLIN) {
            int buf = 0;
            read(m_eventfd, &buf, sizeof(uint64_t));
            if (buf == m_eventfd) {
                m_exit = true;
            }
        }
    } else if (m_fd2Handler.count(fd) > 0) {
        auto handler = m_fd2Handler[fd];
        if (event.events & EPOLLIN) {
            // todo ||
            handler->read(m_threadPool);
        }

        if (event.events & EPOLLOUT) {
            handler->write();
        }

        if (event.events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)) {
            m_fd2Handler.erase(fd);
            m_epoll->removefd(fd);
        }
    }
}

void Reactor::release() {
    for (auto& [fd, handler] : m_fd2Handler) {
        m_epoll->removefd(fd);
    }
    close(m_listenfd);
}