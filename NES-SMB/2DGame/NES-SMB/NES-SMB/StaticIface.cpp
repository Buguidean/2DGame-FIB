#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "StaticIface.h"
#include "Game.h"

enum anims {
	STAY
};

void StaticIface::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spriteSheetStatic)
{
	sprite = Sprite::createSprite(glm::ivec2(512, 32), glm::vec2(1.f, 1.f), &spriteSheetStatic, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(STAY, 1);
	sprite->addKeyframe(STAY, glm::vec2(0.f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posStatic.x), float(tileMapDispl.y + posStatic.y)));
}

void StaticIface::setPosition(const glm::vec2 &pos)
{
	posStatic = pos;
}

glm::fvec2 StaticIface::getPosition()
{
	return posStatic;
}

void StaticIface::update(int deltaTime)
{
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posStatic.x), float(tileMapDispl.y + posStatic.y)));
}


void StaticIface::render()
{
	sprite->render();
}