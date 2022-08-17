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
    auto status = processRead();
    processWrite(status);
    m_bufferOut->buffer = m_buffer.rdbuf();
    // reset event to trigger
    m_epoll->addEvent(m_fd, EPOLLOUT);
}   

Processor::RequestStatus Processor::processRead() {
    istringstream in((char*)m_bufferIn->buffer);

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

    return status;
    // string body;
    // getline(in, body);
    // parseRequestBody(body);
}

Processor::RequestStatus Processor::parseRequestLine(const string& line) {
    // deal with Http 1.1
    int firstSpace = line.find_first_of(' ');
    int secondSpace = line.find_first_of(' ', firstSpace + 1);

    m_requestFile = line.substr(firstSpace + 1, secondSpace - firstSpace - 1);
    if (m_requestFile == "/") {
        m_requestFile += DefaultPage;
    }

    string filePath = m_rootDir + m_requestFile;
    if (S_ISDIR(m_fileStat.st_mode) || stat(filePath.c_str(), &m_fileStat) < 0) {
        return NOREQUEST;
    }

    return FILEREQUEST;
}

Processor::RequestStatus Processor::parseRequestHeader(const vector<string>& headers) {
    return NOREQUEST;
}

Processor::RequestStatus Processor::parseRequestBody(const string& body) {
    return NOREQUEST;
}

void Processor::processWrite(RequestStatus status) {
    switch(status) {
        case FILEREQUEST: {
            int statusCode = 200;
            writeStatusLine(statusCode);
            writeHeader(m_fileStat.st_size);
            mapFile();  
        }
        default: {
            m_bufferOut->len = -1;
            break;
        }
    }
}

void Processor::writeStatusLine(int statusCode) {
    if (m_statusCode2Title.count(statusCode) == 0) {
        return;
    }

    m_buffer << "HTTP/1.0 " << statusCode << " " << m_statusCode2Title[statusCode] << endl;
}

void Processor::writeHeader(int contentLength) {
    writeContentLength(contentLength);
    writeBlankLine();
}

void Processor::writeContentLength(int contentLength) {
    m_buffer << "Content-Length:" << contentLength << endl;
    m_buffer << "Connection:close" << endl;
    m_buffer << "Content-Type: text/html;charset=utf-8" << endl;
}

void Processor::writeBlankLine() {
    m_buffer << endl;
}

void Processor::writeContent(int statusCode) {
    if (m_statusCode2Title.count(statusCode) == 0) {
        return;
    }

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
    close(m_fd);
}

void Processor::unmapFile() {
    if (m_bufferOutFile->buffer) {
        munmap(m_bufferOutFile->buffer, m_bufferOutFile->len);
        m_bufferOutFile->buffer = nullptr;
    }
}