#include <sys/socket.h>

#include "Handler.h"
#include "../Processor/Processor.h"

using namespace std;

Handler::Handler(int fd) {
    m_fd = fd;
}

void Handler::read() {
    int bytes = recv(m_fd, m_bufferIn, MaxBufferSize, 0);
    if (bytes <= 0)
    {
        return;
    }

    
}

void Handler::write() {
    
}