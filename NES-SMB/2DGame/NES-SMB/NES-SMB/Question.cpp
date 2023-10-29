#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Question.h"
#include "Game.h"


enum QuestionAnims
{
	ACTIVE, INACTIVE
};


void Question::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	spritesheet.loadFromFile("images/block_animations.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0625f, 0.0625f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(ACTIVE, 8);
	sprite->addKeyframe(ACTIVE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(ACTIVE, glm::vec2(0.0625f, 0.f));
	sprite->addKeyframe(ACTIVE, glm::vec2(0.125f, 0.f));

	sprite->setAnimationSpeed(INACTIVE, 8);
	sprite->addKeyframe(INACTIVE, glm::vec2(0.1875f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
}

void Question::update(int deltaTime)
{
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
}

void Question::render()
{
	sprite->render();
}

void Question::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Question::setPosition(const glm::vec2 &pos)
{
	posBlock = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
}

glm::fvec2 Question::getPosition()
{
	return posBlock;
}
