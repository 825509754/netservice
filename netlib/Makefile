
SRCS := netsession.cpp reactorcore.cpp application.cpp
OBJS := $(SRCS:%.cpp=%.o)
#OBJS = netsession.o application.o reactorcore.o
CXXFLAGS = -std=c++11 -g -Wall -O2

LIB_NET_DIR := ../lib

LIB_NET = libnet.a

$(LIB_NET):$(OBJS)
	ar rc $(LIB_NET)

netsession.o:netsession.cpp netsession.h
	$(CXX) $(CXXFLAGS) -c netsession.cpp -o netsession.o

reactorcore.o:reactorcore.cpp reactorcore.h service.h
	$(CXX) $(CXXFLAGS) -c reactorcore.cpp -o reactorcore.o
	
application.o:application.cpp application.h
	$(CXX) $(CXXFLAGS) -c application.cpp -o application.o

.PHONY: clean
clean:
	rm -f *.o


