#ifndef __ReactorET_h__
#define __ReactorET_h__
#include "Reactor.h"

class ReactorET : public Reactor {
public:
    using Reactor::Reactor;
    
    virtual void initAcceptor();

    virtual void triggerRead(const std::shared_ptr<Handler>& handler);

    virtual int triggerWrite(const std::shared_ptr<Handler>& handler);
};

#endif