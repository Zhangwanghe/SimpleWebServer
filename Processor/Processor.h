#ifndef __Processor_h__
#define __Processor_h__
#include "../ThreadPool/Runnable.h"
#include "../public.h"
#include <memory>
#include <string>
#include <vector>

class Processor : public Runnable{
    std::shared_ptr<Buffer> m_bufferIn;
    std::shared_ptr<Buffer> m_bufferOut;

    void processRead();

    void parseRequestLine(const std::string& line);

    void parseRequestHeader(const std::vector<std::string>& headers);

    void parseRequestBody(const std::vector<std::string>& s);

public:
    Processor(const std::shared_ptr<Buffer>& in, const std::shared_ptr<Buffer>& out);

    virtual void run();
};

#endif