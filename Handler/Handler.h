#ifndef __Handler_h__
#define __Handler_h__
#include "../public/public.h"
#include "../Processor/Processor.h"
#include "../ThreadPool/IThreadPool.h"
#include "../public/Epoll.h"
#include <memory>

class Handler {
    char m_buffer[MaxBufferSize] ;
    std::shared_ptr<Buffer> m_bufferOut;
    std::shared_ptr<Buffer> m_bufferOutFile;

    bool m_isHandling = false;
    
protected:
    int m_fd;

    std::shared_ptr<Epoll> m_epoll;

    std::shared_ptr<Buffer> m_bufferIn;

    std::shared_ptr<Runnable> m_processor;

public:
    Handler(int fd, const std::shared_ptr<Epoll>& epoll);

    virtual bool read(const std::shared_ptr<IThreadPool>& threadPool);

    bool write();

    bool writeFile(struct iovec& iv);
};

#endif