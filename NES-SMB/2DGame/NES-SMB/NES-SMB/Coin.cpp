#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Coin.h"
#include "Game.h"

#define FALL_STEP 4

enum CoinAnims
{
	ACTIVE
};


void Coin::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spritesheet)
{
	//spritesheet.loadFromFile("images/dropCoins.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.5f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(ACTIVE, 8);
	sprite->addKeyframe(ACTIVE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(ACTIVE, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(ACTIVE, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(ACTIVE, glm::vec2(0.5f, 0.5f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPower.x), float(tileMapDispl.y + posPower.y)));

	vy = 0.f;
	vx = 0.f;
	should_render = false;
	poping = false;
	picked = -1;
	coin = true;
	startY = posPower.y;
}

void Coin::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (poping) {
		if ((startY - posPower.y) >= 76) {
			vy -= 0.08 * deltaTime;
		}
		int dv = int(vy);
		posPower.y -= dv;
		if ((posPower.y >= startY - 64.f) && (vy < 0.f)){
			poping = false;
			picked = 3;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPower.x), float(tileMapDispl.y + posPower.y)));
}

void Coin::set_render(const bool state)
{
	should_render = state;
}

void Coin::set_poping(const bool state)
{
	poping = state;
	setPosition(glm::vec2(posPower.x + 8, posPower.y));
	startY = posPower.y;
	vy = 10.0f;
}

bool Coin::get_render()
{
	return should_render;
}

int Coin::is_picked()
{
	return picked;
}

bool Coin::is_coin()
{
	return coin;
}