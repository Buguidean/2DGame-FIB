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
	bumping = false;
	startY = posBlock.y;
}

void Question::sprite_update(int deltaTime) {
	sprite->update(deltaTime);
}

void Question::update(int deltaTime)
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

bool Question::not_bumping()
{
	return !bumping;
}
