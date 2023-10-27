#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 134
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_RIGHT, JUMP_LEFT, TURN_LEFT, TURN_RIGHT, DEATH
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	Moving = false;
	marg = false;
	apex = false;
	vx = 0.f;
	vy = 0.f;
	spritesheet.loadFromFile("images/small_mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(9);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.5f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.5f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.375f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.125, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.375, 0.f));

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.625f, 0.5f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.625f, 0.f));

		sprite->setAnimationSpeed(TURN_LEFT, 8);
		sprite->addKeyframe(TURN_LEFT, glm::vec2(0.5f, 0.5f));

		sprite->setAnimationSpeed(TURN_RIGHT, 8);
		sprite->addKeyframe(TURN_RIGHT, glm::vec2(0.5f, 0.f));

		sprite->setAnimationSpeed(DEATH, 8);
		sprite->addKeyframe(DEATH, glm::vec2(0.75f, 0.f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	oldPlayer = posPlayer;
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	oldPlayer = posPlayer;

	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (vx < -2.90f)
			sprite->setAnimationSpeed(MOVE_LEFT, 14);
		else
			sprite->setAnimationSpeed(MOVE_LEFT, 8);
		if (vx <= 0) {
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
		}

		else {
			posPlayer.x += int(vx);
			sprite->changeAnimation(TURN_LEFT);
		}

		if (vx > -3.f) {
			if (bJumping)
				vx -= 0.03f * deltaTime / 5.f;
			else
				vx -= 0.01f * deltaTime;
		}

		int dv = int(vx);
		posPlayer.x += dv;
	}

	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (vx > 2.90f)
			sprite->setAnimationSpeed(MOVE_RIGHT, 14);
		else
			sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		if (vx >= 0) {
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
		}
		else {
			posPlayer.x += int(vx);
			sprite->changeAnimation(TURN_RIGHT);
		}

		if (vx < 3.f) {
			if (bJumping)
				vx += 0.03f * deltaTime / 5.f;
			else
				vx += 0.01f * deltaTime;
		}

		int dv = int(vx);
		posPlayer.x += dv;
	}

	else
	{
		posPlayer.x += int(vx);
		if (vx < 2.0f)
			sprite->setAnimationSpeed(MOVE_RIGHT, 4); 
		if (vx > -2.0f)
			sprite->setAnimationSpeed(MOVE_LEFT, 4);
		if ((sprite->animation() == MOVE_LEFT) && vx == 0.0f)
			sprite->changeAnimation(STAND_LEFT);
		else if ((sprite->animation() == MOVE_RIGHT) && vx == 0.0f)
			sprite->changeAnimation(STAND_RIGHT);
	}

	if (std::abs(vx) > 0.01f) {
		// Simulate friction by reducing the velocity
		if (vx > 0.0f) {
			if (bJumping)
				vx -= 0.01f * deltaTime / 10.f;
			else
				vx -= 0.004f * deltaTime;
			if (vx < 0.0f) {
				vx = 0.0f;
			}
		}
		else if (vx < 0.0f) {
			if (bJumping)
				vx += 0.01f * deltaTime / 10.f;
			else
				vx += 0.004f * deltaTime;
			if (vx > 0.0f) {
				vx = 0.0f;
			}
		}
	}
	else {
		vx = 0.0f;
	}

	//////////////// Collision Left/Right /////////////////////////////////////////////////////////
	if (vx >= 0.f) {
		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32), &posPlayer.x)) {
			vx = 0.0f;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}

	else {
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32), &posPlayer.x, marg, ic)) {
			vx = 0.0f;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////

	if (bJumping)
	{
		if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT)
			sprite->changeAnimation(JUMP_LEFT);
		else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT)
			sprite->changeAnimation(JUMP_RIGHT);

		if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			/*
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			*/

			if (!apex) {
				if (abs(startY - posPlayer.y) >= 130)
					apex = true;
				if (vy < 3.6f)
					vy += 0.4f * deltaTime;

			}
			else if (apex && vy > -5.f) {
				vy -= 0.08f * deltaTime;
			}

			int dv = int(vy);
			posPlayer.y -= dv;

			if (vy > 0.f)
				bJumping = !map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
			else
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
		}
		else {
			apex = true;
			if (abs(startY - posPlayer.y) >= 60)
				if (vy > -5.f)
					vy -= 0.08f * deltaTime;
			int dv = int(vy);
			posPlayer.y -= dv;
			if (vy > 0.f)
				bJumping = !map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
			else
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
		}
	}
	else
	{
		apex = false;
		vy = 0.f;
		posPlayer.y += FALL_STEP;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			if (sprite->animation() == JUMP_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == JUMP_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);

			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition()
{
	return posPlayer;
}

float Player::getVelocity()
{
	return vx;
}

bool Player::moving()
{
	return oldPlayer.x < posPlayer.x;
}

bool Player::moving_up()
{
	return oldPlayer.y > posPlayer.y;
}

void Player::setVelocity()
{
	vx = 0.f;
}

void Player::margin(bool value, int center)
{
	marg = value;
	ic = center;
}

void Player::killAnimation() {
	sprite->changeAnimation(DEATH);
}
