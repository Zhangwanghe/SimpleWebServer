#ifndef __Processor_h__
#define __Processor_h__
#include "../ThreadPool/Runnable.h"
#include "../public.h"
#include <memory>

class Processor : public Runnable{
    std::shared_ptr<Buffer> m_bufferIn;
    std::shared_ptr<Buffer> m_bufferOut;

public:
    Processor(const std::shared_ptr<Buffer>& in, const std::shared_ptr<Buffer>& out);

    virtual void run();
};

#endif