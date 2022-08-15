#ifndef __Processor_h__
#define __Processor_h__
#include "../ThreadPool/Runnable.h"
#include "../public/public.h"
#include "../public/Epoll.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

class Processor : public Runnable{
    std::shared_ptr<Buffer> m_bufferIn;
    std::shared_ptr<Buffer> m_bufferOut;
    std::shared_ptr<Buffer> m_bufferOutFile;
        
    std::ostringstream m_buffer;
    std::string m_rootDir;
    std::string m_requestFile;
    std::unordered_map<int, std::string> m_statusCode2Title;

    std::shared_ptr<Epoll> m_epoll;
    
    enum MethodType {
        GET = 0,
        POST
    };

    enum RequestStatus {
        BADREQUEST,
        NOREQUEST,
        FILEREQUEST
    };

    void initStatusCode();

    RequestStatus processRead();

    RequestStatus parseRequestLine(const std::string& line);

    RequestStatus parseRequestHeader(const std::vector<std::string>& headers);

    RequestStatus parseRequestBody(const std::string& s);

    void processWrite(RequestStatus status);

    void writeStatusLine(int status);

    void writeHeader(int contentLength);

    void writeContentLength(int contentLength);

    void writeBlankLine();

public:
    Processor(const std::shared_ptr<Buffer>& in, const std::shared_ptr<Buffer>& out,
            const std::shared_ptr<Buffer>& outFile, const std::shared_ptr<Epoll>& epoll, std::string rootDir = "static");

    virtual void run();
};

#endif