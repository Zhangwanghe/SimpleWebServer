#ifndef __AcceptorET_h__
#define __AcceptorET_h__
#include "Acceptor.h"

class Handler;
class AcceptorET : public Acceptor {
public:
    virtual std::optional<std::pair<int, std::shared_ptr<Handler>>> acceptConnect(int listenfd, const std::shared_ptr<Epoll>& epoll);
};

#endif