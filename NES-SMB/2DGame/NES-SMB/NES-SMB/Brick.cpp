#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Brick.h"
#include "Game.h"


enum BrickAnims
{
	ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE
};


void Brick::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	spritesheet.loadFromFile("images/text_tileset.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(10);

	sprite->setAnimationSpeed(ZERO, 8);
	sprite->addKeyframe(ZERO, glm::vec2(0.f, 0.0f));

	sprite->setAnimationSpeed(ONE, 8);
	sprite->addKeyframe(ONE, glm::vec2(0.0625f, 0.0f));

	sprite->setAnimationSpeed(TWO, 8);
	sprite->addKeyframe(TWO, glm::vec2(0.125f, 0.0f));

	sprite->setAnimationSpeed(THREE, 8);
	sprite->addKeyframe(THREE, glm::vec2(0.1875f, 0.0f));

	sprite->setAnimationSpeed(FOUR, 8);
	sprite->addKeyframe(FOUR, glm::vec2(0.25f, 0.0f));

	sprite->setAnimationSpeed(FIVE, 8);
	sprite->addKeyframe(FIVE, glm::vec2(0.3125f, 0.0f));

	sprite->setAnimationSpeed(SIX, 8);
	sprite->addKeyframe(SIX, glm::vec2(0.375f, 0.0f));

	sprite->setAnimationSpeed(SEVEN, 8);
	sprite->addKeyframe(SEVEN, glm::vec2(0.4375f, 0.0f));

	sprite->setAnimationSpeed(EIGHT, 8);
	sprite->addKeyframe(EIGHT, glm::vec2(0.5f, 0.0f));

	sprite->setAnimationSpeed(NINE, 8);
	sprite->addKeyframe(NINE, glm::vec2(0.5625f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
}

void Brick::update(int deltaTime)
{
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
}

void Brick::render()
{
	sprite->render();
}

void Brick::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Brick::setPosition(const glm::vec2 &pos)
{
	posBlock = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
}

glm::fvec2 Brick::getPosition()
{
	return posBlock;
}
