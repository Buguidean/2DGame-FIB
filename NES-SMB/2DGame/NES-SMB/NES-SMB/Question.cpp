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


void Question::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &animated_blocks)
{
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0625f, 0.0625f), &animated_blocks, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(ACTIVE, 6);
	sprite->addKeyframe(ACTIVE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(ACTIVE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(ACTIVE, glm::vec2(0.f, 0.f));
	sprite->setAnimationSpeed(ACTIVE, 8);
	sprite->addKeyframe(ACTIVE, glm::vec2(0.0625f, 0.f));
	sprite->addKeyframe(ACTIVE, glm::vec2(0.125f, 0.f));
	sprite->addKeyframe(ACTIVE, glm::vec2(0.0625f, 0.f));

	sprite->setAnimationSpeed(INACTIVE, 5);
	sprite->addKeyframe(INACTIVE, glm::vec2(0.1875f, 0.f));

	sprite->changeAnimation(ACTIVE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
	
	vy = 0.f;
	question = true;
	used = false;
	gift = true;
	bumping = false;
	startY = posBlock.y;
}

bool Question::check_colision() {
	int state = map->collisionMarioBlock(playerPos, marioSpriteSize, posBlock, glm::ivec2(32, 32));
	if (state == 0)
		return true;
	else
		return false;
}

void Question::sprite_update(int deltaTime) {
	sprite->update(deltaTime);
}

void Question::update(int deltaTime)
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
				sprite->changeAnimation(INACTIVE);
				break;
			case -1:
				break;
			}
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
	}
}

bool Question::not_bumping()
{
	return !bumping;
}

void Question::set_used()
{
	used = true;
}

void Question::set_inactive()
{
	sprite->changeAnimation(INACTIVE);
}

void Question::set_gift()
{
	gift = true;
}

bool Question::get_gift()
{
	return gift;
}

bool Question::is_question()
{
	return question;
}