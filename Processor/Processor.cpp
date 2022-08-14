#include "Processor.h"
#include <sstream>
#include <iostream>
using namespace std;

Processor::Processor(const std::shared_ptr<Buffer>& in, const std::shared_ptr<Buffer>& out) {
    this->m_bufferIn = in;
    this->m_bufferOut = out;
}

void Processor::run() {
    processRead();
}   

void Processor::processRead() {
    istringstream in(m_bufferIn->buffer);

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

    // string body;
    // getline(in, body);
    // parseRequestBody(body);
}

void Processor::parseRequestLine(const string& line) {

}

void Processor::parseRequestHeader(const vector<string>& headers) {

}

void Processor::parseRequestBody(const vector<string>& body) {

}

