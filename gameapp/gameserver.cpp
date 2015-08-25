#include "gameapp.h"
#include <stdlib.h>

#include "socketservice.h"

int main(int argc, char **argv)
{
	GameApp app("game");
	if (!app.init())
	{
		printf("app init failed!\n");
		return -1;
	}
		
		
	SocketService *service = new SocketService("127.0.0.1", 50000);
	app.add_service(service);
	app.start_service();
	
	return 0;
}