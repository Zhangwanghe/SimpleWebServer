CXX ?= g++ 

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS += -g
else
    CXXFLAGS += -O2

endif

server: main.cpp  ./Reactor/Reactor.cpp ./Acceptor/Acceptor.cpp ./Handler/Handler.cpp ./Processor/Processor.cpp ./Reactor/ReactorET.cpp ./Acceptor/AcceptorET.cpp ./Handler/HandlerET.cpp ./ThreadPool/ThreadPoolStatic/ThreadPoolStatic.cpp ./ThreadPool/ThreadPoolStatic/WorkerStatic.cpp ./ThreadPool/ThreadPoolDynamic/ThreadPoolDynamic.cpp ./ThreadPool/ThreadPoolDynamic/WorkerDynamic.cpp ./ThreadPool/Runnable.cpp ./ThreadPool/IThreadPool.cpp  ./public/Epoll.cpp
	$(CXX) -std=c++2a -o server $^ $(CXXFLAGS) -lpthread

clean:
	rm  -r server