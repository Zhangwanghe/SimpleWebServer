#include "Handler.h"
#include "../Processor/Processor.h"
#include <sys/socket.h>
#include <iostream>

using namespace std;

Handler::Handler(int fd) {
    m_fd = fd;
    m_bufferIn = make_shared<Buffer>();
    m_bufferOut = make_shared<Buffer>();
    m_processer = make_shared<Processor>(m_bufferIn, m_bufferOut);
}

void Handler::read(shared_ptr<ThreadPool> threadPool) {
    int bytes = recv(m_fd, m_bufferIn->buffer, MaxBufferSize, 0);
    if (bytes <= 0)
    {
        return;
    }

    threadPool->schedule(m_processer);
}

void Handler::write() {
    
}