#ifndef __Runnable_h__
#define __Runnable_h__

class Runnable {
public:
    virtual void run() = 0;

    virtual void clear() = 0;

    virtual ~Runnable() = 0;
};

#endif