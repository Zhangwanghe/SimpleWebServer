#include <iostream>
#include "Reactor/Reactor.h"

const int port = 10086;

int main() {
    Reactor reactor;
    reactor.init(port);
    reactor.startup();
}