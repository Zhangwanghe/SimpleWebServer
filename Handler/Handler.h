#ifndef __Handler_h__
#define __Handler_h__
#include "../public.h"

class ThreadPool;
class Handler {
    int m_fd;

    char m_bufferIn[MaxBufferSize];
    char m_bufferOut[MaxBufferSize];

public:
    Handler(int fd);

    void read(ThreadPool* threadPool);

    void write();
};

#endif