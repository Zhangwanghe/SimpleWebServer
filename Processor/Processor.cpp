#include "Processor.h"
#include <sstream>
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

Processor::Processor(const shared_ptr<Buffer>& in, const shared_ptr<Buffer>& out,
                     const shared_ptr<Buffer>& outFile, const shared_ptr<Epoll>& epoll,
                     int fd, string rootDir) {
    m_bufferIn = in;
    m_bufferOut = out;
    m_bufferOutFile = outFile;
    m_bufferOut->buffer = m_buffer.rdbuf();

    char   buffer[MaxBufferSize];   
    getcwd(buffer, MaxBufferSize);
    m_rootDir = buffer;
    m_rootDir += "/" + rootDir;

    m_epoll = epoll;
    m_fd = fd;

    initStatusCode();
}

void Processor::initStatusCode() {
    m_statusCode2Title[200] = "OK";
    m_statusCode2Title[400] = "Bad Request";
    m_statusCode2Title[403] = "Forbidden";
    m_statusCode2Title[404] = "Not Found";
    m_statusCode2Title[500] = "Internal Error";
}

void Processor::run() {
    processRead();
    processWrite();
    // reset event to trigger
    m_epoll->addEvent(m_fd, EPOLLOUT);
}   

void Processor::processRead() {
    istringstream in;
    {
        lock_guard<recursive_mutex> g(m_bufferIn->mutex);
        in = istringstream(static_cast<char*>(m_bufferIn->buffer));
    }

    string line;
    getline(in, line);
    auto status = parseRequestLine(line);

    vector<string> headers;
    while (true) {
        string header;
        getline(in, header);
        
        if (header.empty()) {
            parseRequestHeader(headers);
            break;
        } 

        headers.push_back(header);
    }

    m_status = status;
}

Processor::RequestStatus Processor::parseRequestLine(const string& line) {
    // deal with Http 1.1
    int firstSpace = line.find_first_of(' ');
    int secondSpace = line.find_first_of('?', firstSpace + 1);
    if (secondSpace == string::npos) {
        secondSpace = line.find_first_of(' ', firstSpace + 1);
    }

    m_requestFile = line.substr(firstSpace + 1, secondSpace - firstSpace - 1);
    if (m_requestFile == "/") {
        m_requestFile += DefaultPage;
    }

    string filePath = m_rootDir + m_requestFile;
    if (S_ISDIR(m_fileStat.st_mode) || stat(filePath.c_str(), &m_fileStat) < 0) {
        return BADREQUEST;
    }

    int versionPos = line.find_last_of("HTTP/");
    if (versionPos != string::npos) {
        m_httpVersion = line.substr(versionPos + 1, 3);
        m_isKeepAlive = m_httpVersion == "1.1" ? true : false;
    }

    return FILEREQUEST;
}

Processor::RequestStatus Processor::parseRequestHeader(const vector<string>& headers) {
    for (auto& header : headers) {
        int splitPos = header.find_first_of(':');
        if (splitPos == string::npos) {
            continue;
        }

        string key = header.substr(0, splitPos);
        string value = header.substr(splitPos + 1);

        if (key == "Connection") {
            m_isKeepAlive = "keep-alive" ? true : false;
        }
    }

    return NOREQUEST;
}

Processor::RequestStatus Processor::parseRequestBody(const string& body) {
    return NOREQUEST;
}

void Processor::processWrite() {
    switch(m_status) {
        case FILEREQUEST: {
            int statusCode = 200;
            writeStatusLine(statusCode);
            writeHeader(m_fileStat.st_size);
            mapFile();  
            break;
        }
        case BADREQUEST: {
            int statusCode = 404;
            writeStatusLine(statusCode);
            writeHeader(m_statusCode2Title[statusCode].length());
            writeContent(statusCode);
            break;
        }
        default: {
            break;
        }
    }
}

void Processor::writeStatusLine(int statusCode) {
    // todo modify status according to input
    m_buffer << "HTTP/1.0 " << statusCode << " " << m_statusCode2Title[statusCode] << endl;
}

void Processor::writeHeader(int contentLength) {
    writeContentLength(contentLength);
    writeBlankLine();
}

void Processor::writeContentLength(int contentLength) {
    m_buffer << "Content-Length:" << contentLength << endl;
}

void Processor::writeBlankLine() {
    m_buffer << endl;
}

void Processor::writeContent(int statusCode) {
    m_buffer << m_statusCode2Title[statusCode] << endl;
}

void Processor::mapFile() {
    string filePath = m_rootDir + m_requestFile;
    int fd = open(filePath.c_str(), O_RDONLY);
    m_bufferOutFile->buffer = mmap(0, m_fileStat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    m_bufferOutFile->len = m_fileStat.st_size;
    close(fd);
}

void Processor::clear() {
    unmapFile();
}

void Processor::unmapFile() {
    if (m_bufferOutFile->buffer) {
        munmap(m_bufferOutFile->buffer, m_bufferOutFile->len);
        m_bufferOutFile->buffer = nullptr;
    }
}

bool Processor::isKeepAlive() {
    return m_isKeepAlive;
}