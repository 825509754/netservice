SRCS := io_buffer.cpp
OBJS := $(SRCS:%.cpp=%.o)

CXXFLAGS = -std=c++11 -g -Wall -O2

LIB_DIR := ../lib/
LIB_CORE = $(LIB_DIR)libcore.a

$(LIB_CORE):$(OBJS)
	ar rc $(LIB_CORE) $(OBJS)

io_buffer.o:io_buffer.cpp io_buffer.h malloc.h lock.h
	$(CXX) $(CXXFLAGS) -c io_buffer.cpp -o io_buffer.o

singleton.o:singleton.cpp singleton.h
	$(CXX) $(CXXFLAGS) -c singleton.cpp -o singleton.o
	
.PHONY:clean
clean:
	rm -f *.o	