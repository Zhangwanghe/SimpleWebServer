#ifndef __Worker_h__
#define __Worker_h__

class ThreadPool;
class Worker {
public:
    void run(ThreadPool* pool);
};

#endif