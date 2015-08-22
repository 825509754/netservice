#netsession.o:netsession.cpp netsession.h
#	cc -std=gnu++0x -c -g netsession.cpp

#application.o:application.cpp application.h
#	cc -std=gnu++0x -c -g application.cpp

#clean:
#	rm -f *.o
OBJS = netsession.o application.o reactorcore.o

LIBEVENT_INC ?=../3rd/libevent/include

libnet.a:$(OBJS)
	g++ ar -rc -std=c++11 -g -O2 libnet.a

netsession.o:netsession.cpp netsession.h
	g++ -std=c++11 -g -O2 -I$(LIBEVENT_INC) netsession.cpp -o netsession.o

application.o:application.cpp appliction.h
	g++ -std=c++11 -g -O2 -I$(LIBEVENT_INC) application -o application.o

reactorcore.o:reactorcore.cpp reactorcore.h
	g++ -std=c++11 -g -O2 -I$(LIBEVENT_INC) reactorcore -o reactorcore.o


