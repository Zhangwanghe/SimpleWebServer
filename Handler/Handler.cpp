#include "Handler.h"
#include "../Processor/Processor.h"
#include "../ThreadPool/ThreadPool.h"

#include <sys/socket.h>
#include <iostream>

using namespace std;

Handler::Handler(int fd) {
    m_fd = fd;
}

void Handler::read(ThreadPool* threadPool) {
    int bytes = recv(m_fd, m_bufferIn, MaxBufferSize, 0);
    if (bytes <= 0)
    {
        return;
    }

    auto processor = new Processor(Buffer(m_bufferIn, bytes), Buffer(m_bufferOut, -1));
    threadPool->schedule(processor);
}

void Handler::write() {
    
}