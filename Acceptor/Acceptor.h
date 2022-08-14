#ifndef __Acceptor_h__
#define __Acceptor_h__
#include "../Handler/Handler.h"
#include <optional>
#include <memory>

class Handler;
class Acceptor {
public:
    std::optional<std::pair<int, std::shared_ptr<Handler>>> accept_connect(int listenfd);
};

#endif