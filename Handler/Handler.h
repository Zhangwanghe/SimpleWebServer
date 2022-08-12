class Handler {
    int m_fd;
public:
    Handler(int fd);

    void read();

    void write();
};