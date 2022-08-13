#include "Acceptor.h"
#include "../Handler/Handler.h"
#include <errno.h>
#include <netinet/in.h>
#include <iostream>
using namespace std;

using namespace std;

optional<pair<int, Handler*>> Acceptor::accept_connect(int listenfd) {
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
    if (connfd < 0) {
        cout << "accept error " << errno << endl;
        return nullopt;
    }

    return make_optional(make_pair(connfd, new Handler(connfd)));
}