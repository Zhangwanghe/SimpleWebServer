#include <iostream>
#include "Reactor/Reactor.h"

// todo read from config or db
const int Port = 10086;

int main() {
    Reactor reactor;
    reactor.init(Port);
    reactor.startup();
    reactor.eventloop();
}