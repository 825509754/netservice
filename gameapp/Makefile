CXXFLAGS = -std=c++11 -g -Wall -O2 -L$(LIB_DIR) -lnet -I"../netlib/"

SRCS := gameserver.cpp gameapp.cpp socketservice.cpp
OBJS := $(SRCS:%.cpp=%.o)

LIB_DIR = ../lib/

Game:$(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o Game

gameserver.o:gameserver.cpp gameapp.cpp gameapp.h socketservice.cpp socketservice.h
	$(CXX) $(CXXFLAGS) -c gameserver.cpp -o gameserver.o

gameapp.o:gameapp.cpp gameapp.h
	$(CXX) $(CXXFLAGS) -c gameapp.cpp -o gameapp.o

socketservice.o:socketservice.cpp socketservice.h
	$(CXX) $(CXXFLAGS) -c socketservice.cpp -o socketservice.o

.PHONY:clean
clean:
	rm -f *.o