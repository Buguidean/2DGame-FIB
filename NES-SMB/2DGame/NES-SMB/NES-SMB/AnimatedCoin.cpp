#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "AnimatedCoin.h"
#include "Game.h"

enum coinAnims {
	ANIMATE
};

void AnimatedCoin::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spriteSheetCoin)
{
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.5f, 0.5f), &spriteSheetCoin, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(ANIMATE, 6);
	sprite->addKeyframe(ANIMATE, glm::vec2(0.f, 0.0f));
	sprite->addKeyframe(ANIMATE, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(ANIMATE, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(ANIMATE, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(ANIMATE, glm::vec2(0.f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCoin.x), float(tileMapDispl.y + posCoin.y)));
}

void AnimatedCoin::setPosition(const glm::vec2 &pos)
{
	posCoin = pos;
}

glm::fvec2 AnimatedCoin::getPosition()
{
	return posCoin;
}

void AnimatedCoin::update(int deltaTime)
{
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCoin.x), float(tileMapDispl.y + posCoin.y)));
}


void AnimatedCoin::render()
{
	sprite->render();
}