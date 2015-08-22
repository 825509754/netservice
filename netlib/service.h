#ifndef SERVICE_H
#define SERVICE_H

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <string>


class service
{
public:
	service(const char* ip, int port):
		ip(ip),
		port(port)
	{
	}


	virtual ~service(){}
	virtual int on_accept(struct bufferevent* event,evutil_socket_t fd,struct sockaddr_in* sa) = 0;


	std::string get_service_ip()
	{
		return ip; 
	}

	int get_service_port()
	{
		return port; 
	}

private:
	std::string ip;
	int         port;

};

#endif