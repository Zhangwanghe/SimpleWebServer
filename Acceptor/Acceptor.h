#ifndef __Acceptor_h__
#define __Acceptor_h__
#include "../Handler/Handler.h"
#include "../public/Epoll.h"
#include <optional>
#include <memory>

class Handler;
class Acceptor {
protected:
    int doAccept(int listenfd);

public:
    virtual std::optional<std::pair<int, std::shared_ptr<Handler>>> acceptConnect(int listenfd, const std::shared_ptr<Epoll>& epoll);
};

#endif