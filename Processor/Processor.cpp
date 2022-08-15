#include "Processor.h"
#include <sstream>
#include <iostream>
using namespace std;

Processor::Processor(const shared_ptr<Buffer>& in, const shared_ptr<Buffer>& out,
                     const shared_ptr<Buffer>& outFile, const shared_ptr<Epoll>& epoll,
                     string rootDir) {
    m_bufferIn = in;
    m_bufferOut = out;
    m_rootDir = rootDir;
    m_bufferOutFile = outFile;
    m_epoll = epoll;

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
}   

Processor::RequestStatus Processor::processRead() {
    istringstream in((char*)m_bufferIn->buffer);

    string line;
    getline(in, line);
    parseRequestLine(line);

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

    return FILEREQUEST;
    // string body;
    // getline(in, body);
    // parseRequestBody(body);
}

Processor::RequestStatus Processor::parseRequestLine(const string& line) {
    // deal with Http 1.1
    m_requestFile = "welcome.html";
}

Processor::RequestStatus Processor::parseRequestHeader(const vector<string>& headers) {
    
}

Processor::RequestStatus Processor::parseRequestBody(const string& body) {

}

void Processor::processWrite(RequestStatus status) {
    switch(status) {
        case FILEREQUEST: {
            
        }
        default:
            break;
    }
}

void Processor::writeStatusLine(int status) {
    if (m_statusCode2Title.count(status) == 0) {
        return;
    }

    m_buffer << "HTTP/1.1 " << status << " " << m_statusCode2Title[status] << endl;
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