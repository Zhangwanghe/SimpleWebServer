#include "ReactorET.h"
#include "../Handler/HandlerET.h"
#include "../Acceptor/AcceptorET.h"
using namespace std;

void ReactorET::initAcceptor() {
    m_acceptor = unique_ptr<Acceptor>(new AcceptorET());
}

void ReactorET::triggerRead(const shared_ptr<Handler>& handler) {
    shared_ptr<HandlerET> handlerET((HandlerET*)handler.get());
    handlerET->setStatus(HandlerRead);
    this->m_threadPool->schedule(shared_ptr<Runnable>((Runnable*)handler.get()));
}

int ReactorET::triggerWrite(const shared_ptr<Handler>& handler) {
    shared_ptr<HandlerET> handlerET((HandlerET*)handler.get());
    handlerET->setStatus(HandlerWrite);
    this->m_threadPool->schedule(shared_ptr<Runnable>((Runnable*)handler.get()));
    return 0;
}