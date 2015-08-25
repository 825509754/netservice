#pragma once
#ifndef _GAME_APP_H_
#define _GAME_APP_H_

#include "../netlib/application.h"

class GameApp : public Appliction
{
public:
	GameApp(std::string name):
		Appliction(name)
	{}
	~GameApp();
	
public:
	virtual bool init();
	virtual bool onUpdate(double elapsed);
	virtual void stop();
	virtual void onTimer(int id);
};

#endif