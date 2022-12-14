#include "Acceptor.h"
#include <errno.h>
#include <netinet/in.h>
#include <iostream>
using namespace std;

optional<pair<int, std::shared_ptr<Handler>>> Acceptor::acceptConnect(int listenfd, const shared_ptr<Epoll>& epoll) {
    int connfd = doAccept(listenfd);
    if (connfd < 0) {
        return nullopt;
    }

    return make_optional(make_pair(connfd, make_shared<Handler>(connfd, epoll)));
}

int Acceptor::doAccept(int listenfd) {
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
    if (connfd < 0) {
        cout << "accept error " << errno << endl;
    }

    return connfd;
}