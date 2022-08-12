CXX ?= g++ 

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS += -g
else
    CXXFLAGS += -O2

endif

server: main.cpp  ./Reactor/Reactor.cpp ./Acceptor/Acceptor.cpp ./Handler/Handler.cpp ./ThreadPool/ThreadPool.cpp ./ThreadPool/Worker.cpp
	$(CXX) -std=c++17 -o server $^ $(CXXFLAGS) 

clean:
	rm  -r server