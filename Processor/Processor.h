#ifndef __Processor_h__
#define __Processor_h__
#include "../ThreadPool/Runnable.h"
#include "../public.h"

class Processor : public Runnable{
    Buffer m_bufferIn;
    Buffer m_bufferOut;
public:
    Processor(const Buffer& in, const Buffer& out);

    virtual void run();
};

#endif