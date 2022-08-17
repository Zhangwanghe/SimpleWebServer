#ifndef __Public_h__
#define __Public_h__
#include <mutex>

const int MaxBufferSize = 2048;

struct Buffer {
    void* buffer;
    int len = -1;
    int used = -1;

    Buffer(int len = -1) {
        this->len = len;
    }
    
    std::recursive_mutex mutex;
};

#endif