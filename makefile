CXX ?= g++ 

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS += -g
else
    CXXFLAGS += -O2

endif

server: main.cpp  ./Reactor/Reactor.cpp ./Acceptor/Acceptor.cpp ./Handler/Handler.cpp ./Processor/Processor.cpp ./ThreadPool/ThreadPool.cpp ./ThreadPool/Worker.cpp ./ThreadPool/Runnable.cpp
	$(CXX) -std=c++2a -o server $^ $(CXXFLAGS) -lpthread

clean:
	rm  -r server