#include <optional>

class Handler;
class Acceptor {
public:
    std::optional<std::pair<int, Handler*>> accept_connect(int listenfd);
};