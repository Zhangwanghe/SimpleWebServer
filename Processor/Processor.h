#ifndef __Processor_h__
#define __Processor_h__
#include "../ThreadPool/Runnable.h"
#include "../public.h"

class Processor : public Runnable{
    Buffer in;
    Buffer out;
public:
    Processor(const Buffer& in, const Buffer& out);

    virtual void run();
};

#endif