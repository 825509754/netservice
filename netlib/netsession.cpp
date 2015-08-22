
#include "netsession.h"
//#include "service.h"
//#include "reactorcore.h"
#include "application.h"

#include <arpa/inet.h>
#include <string.h>

#define INVALID_SOCKET -1

static inline void event_read(struct bufferevent* buf_event, void *data)
{
	net_session *sock = (net_session*)data;
	if (sock == nullptr)
		return;

	sock->do_read();

	return;
}

static inline void event_write(struct bufferevent* buf_event, void *data)
{
	net_session *sock = (net_session*)data;
	if (sock == nullptr)
		return;

	sock->do_write();
}

static inline void event_cb(struct bufferevent* buf_event, short event, void *data)
{
	net_session *sock = (net_session*)data;
	if (sock == nullptr)
		return;

	sock->on_event(event);
}


net_session::net_session()
{
	online = false;
	connecting = false;
	buf_event = nullptr;
}


net_session::~net_session()
{
	do_close();
}


int net_session::connect(const char* ip, unsigned short port)
{
	ReactorCore* rector = Singleton<ReactorCore>::get_instance();
	
	sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof(sockaddr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(port);
	addr_in.sin_addr.s_addr = inet_addr(ip);

	mAddr = &addr_in;

	buf_event = bufferevent_socket_new(rector->get_event_base(), -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE | BEV_OPT_DEFER_CALLBACKS);
	int resutl = bufferevent_socket_connect(buf_event, (struct sockaddr*)&addr_in, sizeof(addr_in));
	if (resutl < 0 )
	{
		evutil_socket_t fd = bufferevent_getfd(buf_event);
		int error = evutil_socket_geterror(fd);
		printf("socket error is %s", evutil_socket_error_to_string(error));
		bufferevent_free(buf_event);

		return -1;
	}

	bufferevent_setcb(buf_event, event_read, event_write, event_cb, this);
	bufferevent_enable(buf_event, EV_READ | EV_WRITE | EV_PERSIST);

	connecting = true;

	return 0;
}

void net_session::close()
{
	do_close();
}


void net_session::clear()
{
	if (buf_event != nullptr)
	{
		bufferevent_free(buf_event);
		bufferevent_disable(buf_event, EV_READ | EV_WRITE | EV_WRITE);

		evutil_socket_t fd = bufferevent_getfd(buf_event);
		if (fd != INVALID_SOCKET)
			evutil_closesocket(fd);

		buf_event = nullptr;
	}
}


void net_session::on_accept(struct bufferevent* bev, evutil_socket_t socket, sockaddr_in* addr)
{
	socket_fd = socket;

	bufferevent_setcb(bev, event_read, event_write, event_cb, this);
	bufferevent_enable(bev, EV_READ|EV_WRITE|EV_PERSIST);

	buf_event = bev;

	online = true;
}

void net_session::do_connect()
{
	connecting = false;
	online = true;
	on_connect();
}


void net_session::do_close()
{
	online = false;
	connecting = false;

	clear();
	on_close();
}

void net_session::do_read()
{
	on_read();
}

void net_session::do_write()
{

}

void net_session::on_event(short event)
{
	evutil_socket_t fd = bufferevent_getfd(buf_event);
	if (event & BEV_EVENT_CONNECTED)
	{
		do_connect();
		return;
	}

	int error = evutil_socket_geterror(fd);
	if (error != 0)
	{
		printf("socket error is %s", evutil_socket_error_to_string(error));
	}

	else if (event & BEV_EVENT_TIMEOUT)
	{
		printf("bufferevent Timed out");
	}
	else if (event & BEV_EVENT_EOF)
	{
		printf("connection closed");
	}
	else if (event & BEV_EVENT_ERROR)
	{
		printf("some other error");
	}

	do_close();
}


int net_session::read(void *data, int len)
{
	if (!is_online())
		return 0;

	evbuffer* input = bufferevent_get_input(buf_event);
	int buflen = evbuffer_get_length(input);
	if (buflen < len)
		return 0;

	int size = bufferevent_read(buf_event, data, len);
	return size;
}

int net_session::send(const void *data, int len)
{
	if (!is_online())
		return 0;

	return bufferevent_write(buf_event, data, len);
}

bool net_session::is_online()
{
	return online; 
}

bool net_session::is_connecting()
{
	return connecting;
}

evutil_socket_t net_session::get_socket_fd() 
{
	return socket_fd;
}




