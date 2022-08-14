#ifndef __Public_h__
#define __Public_h__

const int MaxBufferSize = 2048;

struct Buffer {
    char buffer[MaxBufferSize];
    int len = -1;

    Buffer(int len = -1) {
        this->len = len;
    }
};

#endif