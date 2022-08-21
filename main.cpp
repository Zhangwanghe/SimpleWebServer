#include <iostream>
#include "Reactor/ReactorET.h"
#include "ThreadPool/ThreadPoolDynamic/ThreadPoolDynamic.h"
#include "ThreadPool/ThreadPoolStatic/ThreadPoolStatic.h"
#include <signal.h>

// todo read from config or db
const int Port = 10086;
const int ThreadCount = 10;
ReactorET reactor;

void initSignal();
void sigHandler(int sig);

int main() {
    initSignal();

    reactor.init(Port, std::shared_ptr<IThreadPool>(new ThreadPoolStatic(ThreadCount)));
    reactor.startup();
    reactor.eventloop();
}

void initSignal() {
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);
    signal(SIGABRT, sigHandler);
}

void sigHandler(int sig) {
    reactor.sigHandler(sig);
}