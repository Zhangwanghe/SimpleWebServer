#ifndef __Handler_h__
#define __Handler_h__
#include "../public.h"
#include <memory>

class ThreadPool;
class Processor;
class Handler {
    int m_fd;
    
    std::shared_ptr<Buffer> m_bufferIn;
    std::shared_ptr<Buffer> m_bufferOut;

    std::unique_ptr<Processor> m_processer;

public:
    Handler(int fd);

    void read(ThreadPool* threadPool);

    void write();
};

#endif