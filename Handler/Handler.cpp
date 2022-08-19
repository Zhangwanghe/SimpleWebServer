#include "Handler.h"
#include "../Processor/Processor.h"
#include <sys/socket.h>
#include <iostream>
#include <sys/uio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <mutex>

using namespace std;

Handler::Handler(int fd, const shared_ptr<Epoll>& epoll) {
    m_fd = fd;
    m_epoll = epoll;
    
    m_bufferIn = make_shared<Buffer>();
    m_bufferOut = make_shared<Buffer>();
    m_bufferOutFile = make_shared<Buffer>();
    m_processor = make_shared<Processor>(m_bufferIn, m_bufferOut, m_bufferOutFile, epoll, fd);
    m_bufferIn->buffer = m_buffer;
}

bool Handler::read(shared_ptr<IThreadPool> threadPool) {
    if (m_isHandling) {
        return true;
    }
    m_isHandling = true; 

    int bytes = -1;
    {
        lock_guard<recursive_mutex> g(m_bufferIn->mutex);
        bytes = recv(m_fd, m_bufferIn->buffer, MaxBufferSize, 0);
    }

    if (bytes <= 0)
    {
        if (errno != EINTR) {
            return false;
        }
    }
    
    threadPool->schedule(m_processor);

    return true;
}

bool Handler::write() {
    string buffer;
    {
        lock_guard<recursive_mutex> g(m_bufferOut->mutex);
        buffer = ((const stringbuf*)m_bufferOut->buffer)->str();
        m_bufferOut->len = 0;
    }

    if (buffer.empty()) {
        return true;
    }
    struct iovec iv[2];
    int count = 1;
    
    iv[0].iov_base = (void*)buffer.c_str();
    iv[0].iov_len = buffer.length();

    if (writeFile(iv[1])) {
        ++count;
    }

    int len = writev(m_fd, iv, count);
    if (len < 0) {
        if (errno == EAGAIN)
        {
            m_epoll->addEvent(m_fd, EPOLLOUT);
            return false;
        }
        else if (errno == EPIPE) {
            return true;
        }
    }

    m_processor->clear();
    m_isHandling = false;

    return true;
}

bool Handler::writeFile(struct iovec& iv) {
    if (m_bufferOutFile->len == -1) {
        return false;
    }
    
    iv.iov_base = m_bufferOutFile->buffer;
    iv.iov_len = m_bufferOutFile->len;

    return true;
}