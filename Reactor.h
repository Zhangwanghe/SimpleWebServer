class Reactor {
    int m_port;
    int m_listenfd;
    int m_epollfd;

    void init_listen();

    void init_epoll();

    void add_epoll(int fd);

public:
    void init(int port);
    
    void startup();
};