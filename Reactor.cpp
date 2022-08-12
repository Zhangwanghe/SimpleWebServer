#include "Reactor.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<string.h>
#include <cassert>

#include <sys/epoll.h>

void Reactor::init(int port) {
    m_port = port;
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
    m_epollfd = epoll_create(0);
    assert(m_epollfd >= 0);
}

void Reactor::add_epoll(int fd) {
    epoll_event event;
    event.data.fd = fd;

    epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &event);
}