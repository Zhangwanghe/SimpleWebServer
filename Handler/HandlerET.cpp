#include "HandlerET.h"
using namespace std;

HandlerET::HandlerET(int fd, const shared_ptr<Epoll>& epoll): Handler(fd, epoll) {
    epoll->setMode(EPOLL_ET);
}

void HandlerET::setStatus(HandlerStatus status) {
    m_status = status;
}

void HandlerET::run() {
    if (m_status == HandlerRead) {
        ((Processor*)m_processor.get())->processRead();
    } else {
        ((Processor*)m_processor.get())->processWrite();
        // todo remove fd as in reactor
    }
}

void HandlerET::clear() {

}