#include "MainScene.h"
#include "Game.h"

MainScene::MainScene()
{
}


MainScene::~MainScene()
{
}

void MainScene::init()
{

}

int MainScene::update(int deltaTime)
{
	if (Game::instance().getKey('d'))
		return 0;
	return 1;
}

void MainScene::render()
{

}