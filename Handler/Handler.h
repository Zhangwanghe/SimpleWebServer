#ifndef __Handler_h__
#define __Handler_h__
#include "../public/public.h"
#include "../Processor/Processor.h"
#include "../ThreadPool/IThreadPool.h"
#include "../public/Epoll.h"
#include <memory>

class Handler {
    int m_fd;
    std::shared_ptr<Epoll> m_epoll;
    
    std::shared_ptr<char> m_buffer;
    std::shared_ptr<Buffer> m_bufferIn;
    std::shared_ptr<Buffer> m_bufferOut;
    std::shared_ptr<Buffer> m_bufferOutFile;

    std::shared_ptr<Runnable> m_processor;

    bool m_isHandling = false;

public:
    Handler(int fd, const std::shared_ptr<Epoll>& epoll);

    bool read(std::shared_ptr<IThreadPool> threadPool);

    bool write();

    bool writeFile(struct iovec& iv);
};

#endif