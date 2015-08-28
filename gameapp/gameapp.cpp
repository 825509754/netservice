#include "gameapp.h"

GameApp::~GameApp()
{

}

bool GameApp::init()
{
	return true;
}

bool GameApp::onUpdate(double elapsed)
{
	printf("frame time %lf", elapsed);
	return true;
}

void GameApp::stop()
{
	
}

void GameApp::onTimer(int id)
{
	
}