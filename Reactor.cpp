#include "Reactor.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include<string.h>
#include <cassert>

void Reactor::init(int port) {
    m_port = port;
}

void Reactor::startup() {
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