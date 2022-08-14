#ifndef __Public_h__
#define __Public_h__

struct Buffer {
    char* buffer = nullptr;
    int len = -1;

    Buffer() {
        
    }

    Buffer(char* buffer, int len) {
        this->buffer = buffer;
        this->len = len;
    }
};

const int MaxBufferSize = 2048;

#endif