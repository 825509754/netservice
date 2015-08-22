#ifndef SOCKET_H
#define SOCKET_H


#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <string>


class net_session
{
public:
	net_session();
	virtual ~net_session();

public:
	virtual void on_connect() = 0;
	virtual void on_close() = 0;
	virtual void on_read() = 0;

public:
	void on_accept(struct bufferevent* bev, evutil_socket_t socket, sockaddr_in* addr);
	void on_event(short event);

public:
	void clear();
	void close();

	bool is_online();
	bool is_connecting();

	int read(void *data, int len);
	int send(const void *data, int len);

	int connect(const char* ip, unsigned short port);

	evutil_socket_t get_socket_fd();

private:
	void do_connect();
	void do_close();

public:
	void do_read();
	void do_write();

private:
	bufferevent*		buf_event;
	evutil_socket_t		socket_fd;
	bool				online;
	bool				connecting;

protected:
	std::string			ip;
	int					port;

	sockaddr_in*		mAddr;
};

#endif