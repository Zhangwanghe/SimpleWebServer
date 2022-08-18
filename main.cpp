#include <iostream>
#include "Reactor/Reactor.h"
#include "ThreadPool/ThreadPoolDynamic/ThreadPoolDynamic.h"
#include "ThreadPool/ThreadPoolStatic/ThreadPoolStatic.h"

// todo read from config or db
const int Port = 10086;
const int ThreadCount = 10;

int main() {
    Reactor reactor;
    reactor.init(Port, std::shared_ptr<IThreadPool>(new ThreadPoolStatic(ThreadCount)));
    reactor.startup();
    reactor.eventloop();
}