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
	GOOMBA_MOVE, GOOMBA_DEATH
};


void Enemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	v = 1.f;
	spritesheet.loadFromFile("images/goomba.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(GOOMBA_DEATH, 4);
	sprite->addKeyframe(GOOMBA_DEATH, glm::vec2(0.0f, 0.5f));

	sprite->setAnimationSpeed(GOOMBA_MOVE, 4);
	sprite->addKeyframe(GOOMBA_MOVE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(GOOMBA_MOVE, glm::vec2(0.5, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	oldEnemy = posEnemy;
}

void Enemy::update(int deltaTime)
{
	sprite->update(deltaTime);
	oldEnemy = posEnemy;
	if (sprite->animation() != GOOMBA_MOVE) {
		sprite->changeAnimation(GOOMBA_MOVE);
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
