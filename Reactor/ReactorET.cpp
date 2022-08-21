#include "ReactorET.h"
#include "../Handler/HandlerET.h"
#include "../Acceptor/AcceptorET.h"
using namespace std;

void ReactorET::initAcceptor() {
    m_acceptor = unique_ptr<Acceptor>(new AcceptorET());
}

void ReactorET::triggerRead(const shared_ptr<Handler>& handler) {
    shared_ptr<HandlerET> handlerET(dynamic_pointer_cast<HandlerET>(handler));
    this->m_threadPool->schedule(static_pointer_cast<Runnable>(handlerET));
}

int ReactorET::triggerWrite(const shared_ptr<Handler>& handler) {
    return false;
}