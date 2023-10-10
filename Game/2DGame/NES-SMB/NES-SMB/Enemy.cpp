#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"


//#define JUMP_ANGLE_STEP 4
//#define JUMP_HEIGHT 100
//#define FALL_STEP 4


enum GoombaAnims
{
	GOOMBA1, GOOMBA2, GOOMBAMOVE, GOOMBADOWN
};


void Enemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	v = 1.f;
	spritesheet.loadFromFile("images/goomba.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.5f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(GOOMBA1, 4);
	sprite->addKeyframe(GOOMBA1, glm::vec2(0.f, 0.0f));

	sprite->setAnimationSpeed(GOOMBA2, 4);
	sprite->addKeyframe(GOOMBA1, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(GOOMBADOWN, 4);
	sprite->addKeyframe(GOOMBADOWN, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(GOOMBAMOVE, 4);
	sprite->addKeyframe(GOOMBAMOVE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(GOOMBAMOVE, glm::vec2(0.5, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	oldEnemy = posEnemy;
}

void Enemy::update(int deltaTime)
{
	sprite->update(deltaTime);
	oldEnemy = posEnemy;
	if (sprite->animation() != GOOMBAMOVE) {
		sprite->changeAnimation(GOOMBAMOVE);
	}
	posEnemy.x -= v;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::render()
{
	sprite->render();
}

void Enemy::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Enemy::setPosition(const glm::vec2 &pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

glm::ivec2 Enemy::getPosition()
{
	return posEnemy;
}
