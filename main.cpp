#include <iostream>
#include "Reactor/Reactor.h"

// todo read from config or db
const int Port = 10086;
const int ThreadCount = 10;

int main() {
    Reactor reactor;
    reactor.init(Port, ThreadCount);
    reactor.startup();
    reactor.eventloop();
}