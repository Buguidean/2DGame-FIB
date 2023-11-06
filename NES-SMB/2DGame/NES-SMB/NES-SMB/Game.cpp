#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	type = 0;
	prev_type = 0;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	sceneM.init();
	sceneP.init();
	sceneC.init();
	//sceneI.init();
}

bool Game::update(int deltaTime)
{
	if (type == 0 && prev_type == 0)
		type = sceneP.update(deltaTime);

	else if (type == 0 && prev_type == 1) {
		sceneP.reset();
		type, prev_type = sceneP.update(deltaTime);
	}
	else if(type == 2){
		type = sceneC.update(deltaTime);
	}
	else if (type == 3) {
		// type = sceneI.update(deltaTime);
	}

	else if (type == 1 && prev_type == 1)
		type = sceneM.update(deltaTime);

	else if (type == 1 && prev_type == 0) {
		sceneM.reset();
		type, prev_type = sceneM.update(deltaTime);
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (type == 0)
		sceneP.render();
	else if (type == 1)
		sceneM.render();
	else if (type == 2)
		sceneC.render();
	else if (type == 3) {
		//sceneI.render();
	}
}

void Game::keyPressed(int key)
{
	if (key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





