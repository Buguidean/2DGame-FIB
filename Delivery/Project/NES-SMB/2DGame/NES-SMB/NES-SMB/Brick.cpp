#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Brick.h"
#include "Game.h"


enum BrickAnims
{
	ACTIVE, INACTIVE
};


void Brick::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &animated_blocks)
{
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0625f, 0.0625f), &animated_blocks, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(ACTIVE, 5);
	sprite->addKeyframe(ACTIVE, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(INACTIVE, 5);
	sprite->addKeyframe(INACTIVE, glm::vec2(0.1875f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));

	vy = 0.f;
	used = false;
	gift = false;
	bumping = false;
	question = false;
	startY = posBlock.y;
}

void Brick::sprite_update(int deltaTime) {
	sprite->update(deltaTime);
}

bool Brick::check_colision() {
	int state = map->collisionMarioBlock(playerPos, marioSpriteSize, posBlock, glm::ivec2(32, 32));
	if (state == 0)
		return true;
	else
		return false;
}

void Brick::update(int deltaTime)
{
	if (!used) {
		int state = map->collisionMarioBlock(playerPos, marioSpriteSize, posBlock, glm::ivec2(32, 32));

		if (bumping) {
			if ((startY - posBlock.y) >= 10)
				vy -= 0.1f * deltaTime;

			int dv = int(vy);
			posBlock.y -= dv;

			if (posBlock.y >= startY) {
				posBlock.y = startY;
				drop = true;
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
}

bool Brick::not_bumping()
{
	return !bumping;
}

void Brick::set_used()
{
	used = true;
}

void Brick::set_inactive()
{
	sprite->changeAnimation(INACTIVE);
}

void Brick::set_gift()
{
	gift = true;
}

bool Brick::get_gift()
{
	return gift;
}

bool Brick::is_question() 
{ 
	return question;
}