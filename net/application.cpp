#include "application.h"
#include "service.h"


Loger* g_core_log = nullptr;

void timeout_cb(evutil_socket_t fd, short event, void *arg)
{
	Appliction *app = (Appliction*)arg;
	if (app == nullptr)
		return;

	app->on_time(fd, event);
}

Appliction::Appliction(std::string name)
{
	string log_name = name;
	log_name += "_net.log";
	g_core_log = LogMgr::get_instance()->createLog(log_name.c_str(), LogLevel::LEVEL_DEBUG);

	service_name = name;
	rector = Singleton<ReactorCore>::get_instance();
	rector->init();

	event_assign(&timeout, rector->get_event_base(), -1, 0, timeout_cb, this);
}

Appliction::~Appliction()
{
	
}

bool Appliction::start_service()
{
	struct timeval newtime, difference;
	double elapsed = 0;
	bool result = 0;

	while (1)
	{
		evutil_gettimeofday(&lasttime, NULL);

		event_base_loop(rector->get_event_base(), EVLOOP_NONBLOCK);
		result = onUpdate(elapsed);
		if (!result)
			break;

		Sleep(1);

		evutil_gettimeofday(&newtime, NULL);
		evutil_timersub(&newtime, &lasttime, &difference);
		elapsed = difference.tv_sec + (difference.tv_usec / 1.0e6);
		lasttime = newtime;
	}

	stop_service();
	
	return false;
}

void Appliction::stop_service()
{
	rector->stop();

	stop();

	g_core_log->debug("stop service");
}

void Appliction::add_service(service *pservice)
{
	int result = rector->add_listener_event(pservice->get_service_ip(), pservice->get_service_port(), pservice);
	if (result< 0)
		return;

	g_core_log->debug("start service, port=%d", pservice->get_service_port());
}


void Appliction::addTimer(int sec, int id)
{
	struct timeval tv;
	evutil_timerclear(&tv);

	tv.tv_sec = sec;
	tv.tv_usec = 0;

	event_assign(&timeout, rector->get_event_base(), id, EV_TIMEOUT, timeout_cb, (void*)this);
	event_add(&timeout, &tv);

	//evtimer_set(&timeout, timeout_cb, this);
	//evtimer_add(&timeout, &tv);
}

void Appliction::on_time(evutil_socket_t fd, short event)
{
	onTimer(int(fd));
}


const char* Appliction::get_name()
{
	return service_name.c_str();
}