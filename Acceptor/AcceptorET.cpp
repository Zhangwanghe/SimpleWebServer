#include "AcceptorET.h"
#include "../Handler/HandlerET.h"
using namespace std;

optional<pair<int, std::shared_ptr<Handler>>> AcceptorET::acceptConnect(int listenfd, const shared_ptr<Epoll>& epoll) {
    int connfd = doAccept(listenfd);
    if (connfd < 0) {
        return nullopt;
    }

    return make_optional(make_pair(connfd, shared_ptr<Handler>(new HandlerET(connfd, epoll))));
}