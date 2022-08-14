#include "Processor.h"
using namespace std;

Processor::Processor(const std::shared_ptr<Buffer>& in, const std::shared_ptr<Buffer>& out) {
    this->m_bufferIn = in;
    this->m_bufferOut = out;
}

void Processor::run() {
    
}   
