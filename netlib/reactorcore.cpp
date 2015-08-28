#include "reactorcore.h"
//#include "application.h"

#include<string.h>

//template<> ReactorCore* Singleton<ReactorCore>::instance = 0;

void on_accept(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *p)
{
	ReactorCore::get_instance()->on_event_accept(listener, fd, sa, socklen, p);
}

void on_log(int severity, const char *msg)
{
	ReactorCore::get_instance()->on_event_log(severity, msg);
}

void on_event_fatal(int error)
{
	ReactorCore::get_instance()->on_event_fatal(error);
}


bool ReactorCore::init()
{	
	set_event_config();
	struct event_config *config = event_config_new();

#ifdef WIN32
	event_config_set_flag( config, EVENT_BASE_FLAG_STARTUP_IOCP );
	evthread_use_windows_threads();
	event_config_set_num_cpus_hint(config, 4);
#else
	event_config_set_flag( config, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST );
#endif
	
	base_event = event_base_new_with_config(config);
	event_config_free(config);
	//base_event = event_base_new();
	if(base_event == nullptr)
		return false;

	return true;
}

int ReactorCore::add_listener_event(std::string ip, int port, service* ls)
{
	if (base_event == nullptr)
		return 0;
	
	sockaddr_in addr_sin;
	
	memset(&addr_sin, 0, sizeof(sockaddr_in));
	
	addr_sin.sin_family = AF_INET;
	addr_sin.sin_addr.s_addr = htonl(0);
	addr_sin.sin_port = htons(port);
	evconnlistener *new_ev_ls = evconnlistener_new_bind(base_event, on_accept, (void*)ls, LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1, (struct sockaddr*)&addr_sin, sizeof(sockaddr_in));

	all_listener.push_back(new_ev_ls);
	return 0;
}

void ReactorCore::start()
{
	event_base_dispatch(base_event);
}

void ReactorCore::stop()
{
	for(auto& itr :all_listener)
	{
		evconnlistener_free(itr);
	}

	event_base_free(base_event);
}


void ReactorCore::start_all_listener()
{
	for(auto& itr :all_listener)
	{
		evconnlistener_enable(itr); 
	}
}

void ReactorCore::stop_all_listener()
{
	for(auto& itr :all_listener)
	{
		evconnlistener_disable(itr); 
	}
}

void ReactorCore::set_event_config()
{
//#ifdef EVENT_DEBUG_MODE
	event_enable_debug_mode();
//#endif

	event_set_log_callback(on_log);

	const char **methods = event_get_supported_methods();
	printf("Starting Libevent %s", event_get_version());
	printf("event base method %s", *event_get_supported_methods());
}

void ReactorCore::on_event_accept(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *p)
{
	struct bufferevent* bev;
	service* ls = (service*)p; 
	bev = bufferevent_socket_new(get_event_base(), fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE | BEV_OPT_DEFER_CALLBACKS);
	if(!bev)
	{
		event_base_loopbreak(get_event_base());
		return;
	}

	int rst = ls->on_accept(bev, fd, (sockaddr_in*)sa);
	if(rst < 0 )
	{
		bufferevent_free(bev);
	}
}

void ReactorCore::on_event_log(int severity, const char *msg)
{
	printf("libevent error %s", msg);
}

void ReactorCore::on_event_fatal(int error)
{
	printf("event fatal error code %d", error);
}
