#ifndef __Handler_h__
#define __Handler_h__
#include "../public/public.h"
#include "../Processor/Processor.h"
#include "../ThreadPool/ThreadPool.h"
#include <memory>

class Handler {
    int m_fd;
    
    std::shared_ptr<char> m_buffer;
    std::shared_ptr<Buffer> m_bufferIn;
    std::shared_ptr<Buffer> m_bufferOut;
    std::shared_ptr<Buffer> m_bufferOutFile;

    std::shared_ptr<Runnable> m_processer;

public:
    Handler(int fd);

    void read(std::shared_ptr<ThreadPool> threadPool);

    void write();
};

#endif