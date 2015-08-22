#ifndef APPLICATION_H
#define APPLICATION_H

#include <event2/event-config.h>
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/event_compat.h>

#include "reactorcore.h"


class service;
class Appliction
{
public:
	Appliction(std::string name);
	virtual ~Appliction();

	virtual bool init()=0;
	virtual bool onUpdate(double elapsed) = 0;
	virtual void stop() = 0;
	virtual void onTimer(int id) = 0;

public:
	bool start_service();
	void stop_service();

	void add_service(service* pservice);
	void addTimer(int sec, int id);

	const char* get_name();

public:
	void on_time(evutil_socket_t fd, short event);

private:
	ReactorCore* rector;
	struct event timeout;
	struct timeval lasttime;

	std::string service_name;

};

#endif