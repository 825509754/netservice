#pragma once
#ifndef _SOCKET_SERVICE_H_
#define _SOCKET_SERVICE_H_

#include "../netlib/service.h"

class SocketService : public service
{
public:
	SocketService(const char* ip,int port);
	~SocketService(){};
	
public:
	virtual int on_accept(struct bufferevent* event,evutil_socket_t fd,struct sockaddr_in* sa);	
};

#endif