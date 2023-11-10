#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Mush.h"
#include "Game.h"

#define FALL_STEP 4

enum MushAnims
{
	ACTIVE
};


void Mush::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spritesheet)
{
	//spritesheet.loadFromFile("images/powerups.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(ACTIVE, 1);
	sprite->addKeyframe(ACTIVE, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPower.x), float(tileMapDispl.y + posPower.y)));

	vy = 0.f;
	vx = 2.f;
	should_render = false;
	poping = false;
	picked = -1;
	coin = false;
	startY = posPower.y;
}

void Mush::update(int deltaTime)
{

	int state = map->collisionMarioEnemy(playerPos, marioSpriteSize, posPower, glm::ivec2(32, 32)).first;
	if (state != -1) {
		picked = 1;
	}

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
		
		if (map->collisionMoveRightEntity(posPower, glm::ivec2(32, 32), &posPower.x)) {
			vx = -vx;
		}
		if (map->collisionMoveLeftEntity(posPower, glm::ivec2(32, 32), &posPower.x)) {
			vx = -vx;
		}
		
		int dv = int(vx);
		posPower.x += dv;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPower.x), float(tileMapDispl.y + posPower.y)));
}

void Mush::set_render(const bool state)
{
	should_render = state;
}

void Mush::set_poping(const bool state)
{
	poping = state;
	startY = posPower.y;
	vy = 1.0f;
}

bool Mush::get_render()
{
	return should_render;
}

int Mush::is_picked()
{
	return picked;
}

bool Mush::is_coin()
{
	return coin;
}