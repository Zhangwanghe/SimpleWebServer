class Reactor {
    int m_port;
    int m_listenfd;

public:
    /*
    *  param: ip, port - create socket
    */
    void init(int port);

    void startup();
};