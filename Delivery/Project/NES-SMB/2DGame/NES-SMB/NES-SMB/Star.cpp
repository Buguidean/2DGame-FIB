#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Star.h"
#include "Game.h"

#define FALL_STEP 4

enum StarAnims
{
	ACTIVE
};


void Star::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spritesheet)
{
	//spritesheet.loadFromFile("images/powerups.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(ACTIVE, 12);
	sprite->addKeyframe(ACTIVE, glm::vec2(0.0f, 0.25f));
	sprite->addKeyframe(ACTIVE, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(ACTIVE, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(ACTIVE, glm::vec2(0.75f, 0.25f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPower.x), float(tileMapDispl.y + posPower.y)));

	vy = 0.f;
	vx = 2.f;
	bJumping = false;
	should_render = false;
	poping = false;
	first_touch = false;
	picked = -1;
	coin = false;
	startY = posPower.y;
}

void Star::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (posPower.y == 384) {
		first_touch = true;
	}

	int state = map->collisionMarioEnemy(playerPos, marioSpriteSize, posPower, glm::ivec2(32, 32)).first;
	if (state != -1) {
		picked = 2;
	}

	if (first_touch) {

		if (bJumping) {
			if ((startY - posPower.y) >= 3) {
				if (vy > -5.f) {
					vy -= 0.01f * deltaTime;
				}
			}

			int dvx = int(vx);
			posPower.x += dvx;

			if (map->collisionMoveRightEntity(posPower, glm::ivec2(32, 32), &posPower.x)) {
				vx = -vx;
			}
			if (map->collisionMoveLeftEntity(posPower, glm::ivec2(32, 32), &posPower.x)) {
				vx = -vx;
			}

			int dvy = int(vy);
			posPower.y -= dvy;

			if (map->collisionMoveUp(posPower, glm::ivec2(32, 32), &posPower.y)) {
				vy = -vy;
			}

			bJumping = !map->collisionMoveDown(posPower, glm::ivec2(32, 32), &posPower.y);
					
		}

		else {
			startY = posPower.y;
			bJumping = true;
			vy = 5.0f;
		}
	}

	else {
		if (poping) {
			if ((startY - posPower.y) <= 32) {
				int dv = int(vy);
				posPower.y -= dv;
			}
			else {
				poping = false;
			}
		}

		else {
			posPower.y += FALL_STEP;
			map->collisionMoveDown(posPower, glm::ivec2(32, 32), &posPower.y);
			int dvx = int(vx);
			posPower.x += dvx;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPower.x), float(tileMapDispl.y + posPower.y)));
}

void Star::set_render(const bool state)
{
	should_render = state;
}

void Star::set_poping(const bool state)
{
	poping = state;
	startY = posPower.y;
	vy = 1.0f;
}

bool Star::get_render()
{
	return should_render;
}

int Star::is_picked()
{
	return picked;
}

bool Star::is_coin()
{
	return coin;
}
