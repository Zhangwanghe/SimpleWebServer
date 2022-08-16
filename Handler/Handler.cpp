#include "Handler.h"
#include "../Processor/Processor.h"
#include <sys/socket.h>
#include <iostream>
#include <sys/uio.h>
#include <sys/mman.h>

using namespace std;

Handler::Handler(int fd, const shared_ptr<Epoll>& epoll) {
    m_fd = fd;
    m_epoll = epoll;
    
    m_bufferIn = make_shared<Buffer>();
    m_bufferOut = make_shared<Buffer>();
    m_bufferOutFile = make_shared<Buffer>();
    m_processer = make_shared<Processor>(m_bufferIn, m_bufferOut, m_bufferOutFile, epoll, fd);
    
    m_buffer = shared_ptr<char>(new char[MaxBufferSize]);
    m_bufferIn->buffer = m_buffer.get();
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
    string buffer = ((stringbuf*)m_bufferOut->buffer)->str();
    if (buffer.empty()) {
        return;
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
        }
    }

    unmapFile();
    return;
}

bool Handler::writeFile(struct iovec& iv) {
    if (m_bufferOutFile->len == -1) {
        return false;
    }
    
    iv.iov_base = m_bufferOutFile->buffer;
    iv.iov_len = m_bufferOutFile->len;

    return true;
}

void Handler::unmapFile() {
    if (m_bufferOutFile->buffer) {
        munmap(m_bufferOutFile->buffer, m_bufferOutFile->len);
        m_bufferOutFile->buffer = nullptr;
    }
}