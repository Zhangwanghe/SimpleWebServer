#include "HandlerET.h"
#include <sys/socket.h>
#include <iostream>
using namespace std;

HandlerET::HandlerET(int fd, const shared_ptr<Epoll>& epoll): Handler(fd, epoll) {
    epoll->setMode(EPOLL_ET);
}

void HandlerET::run() {
    read(shared_ptr<IThreadPool>(nullptr));
    ((Processor*)m_processor.get())->processRead();    
    ((Processor*)m_processor.get())->processWrite();
    write();
    m_epoll->addEvent(m_fd, EPOLLOUT);
}

bool HandlerET::read(const std::shared_ptr<IThreadPool>& threadPool) {
    int bytes = -1, index = 0;
    {
        lock_guard<recursive_mutex> g(m_bufferIn->mutex);
        
        while (true) {
            bytes = recv(m_fd, m_bufferIn->buffer + index, MaxBufferSize, 0);
            if (bytes < 0)
            {   
                if (errno == EAGAIN) {
                    break;
                }

                if (errno != EINTR) {
                    return false;
                }
            } else if (bytes == 0) {
                break;
            }

            index += bytes;
        }
    }
  
    return true;
}

void HandlerET::clear() {

}