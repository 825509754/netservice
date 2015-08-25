#include "socketservice.h"

SocketService::SocketService(const char* ip,int port):
service(ip, port)
{
	
}

int SocketService::on_accept(struct bufferevent* event,evutil_socket_t fd,struct sockaddr_in* sa)
{
	return 0;
}