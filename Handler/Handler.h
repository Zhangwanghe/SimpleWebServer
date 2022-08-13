#ifndef __Handler_h__
#define __Handler_h__

struct Buffer {
    char* buffer;
    int len;

    Buffer(char* buffer, int len) {
        this->buffer = buffer;
        this->len = len;
    }
};

class Handler {
    int m_fd;

    static const int MaxBufferSize = 2048;
    char m_bufferIn[MaxBufferSize];

public:
    Handler(int fd);

    void read();

    void write();
};

#endif