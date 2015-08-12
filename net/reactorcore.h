#ifndef REATOR_CORE_H
#define REATOR_CORE_H

#include <event2/thread.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <vector>
#include <string>

#include "singleton.h"


class service;
class ReactorCore: public Singleton<ReactorCore>
{
public:
	friend Singleton<ReactorCore>;

public:
	bool init();

	event_base* get_event_base(){return base_event;}

	void start();
	void stop();

	int add_listener_event(std::string ip, int port, service* ls);

	void start_all_listener();
	void stop_all_listener();

	void set_event_config();

public:
	void on_event_accept(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *p);
	void on_event_log(int severity, const char *msg);
	void on_event_fatal(int error);

public:
	void log_out(int level, const char* msg);

private:
	event_base* base_event;

	std::vector<evconnlistener*> all_listener;
};

#endif