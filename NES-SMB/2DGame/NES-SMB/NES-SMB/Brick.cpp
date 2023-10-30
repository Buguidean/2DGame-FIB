#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Brick.h"
#include "Game.h"


enum BrickAnims
{
	INACTIVE
};


void Brick::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/block_animations.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0625f, 0.0625f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(INACTIVE, 5);
	sprite->addKeyframe(INACTIVE, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));

	vy = 0.f;
	bumping = false;
	startY = posBlock.y;
}

void Brick::sprite_update(int deltaTime) {
	sprite->update(deltaTime);
}

void Brick::update(int deltaTime)
{
	int state = map->collisionMarioBlock(playerPos, marioSpriteSize, posBlock, glm::ivec2(32, 32));

	if (bumping) {
		if ((startY - posBlock.y) >= 10)
			vy -= 0.1f * deltaTime;

		int dv = int(vy);
		posBlock.y -= dv;

		if (posBlock.y >= startY) {
			posBlock.y = startY;
			bumping = false;
		}
	}

	else {
		switch (state)
		{
		case 0:
			vy = 2.0f;
			startY = posBlock.y;
			bumping = true;
			break;
		case -1:
			break;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
}

bool Brick::not_bumping()
{
	return !bumping;
}
