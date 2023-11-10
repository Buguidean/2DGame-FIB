#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "PointsSprite.h"
#include "Game.h"

enum PointsAnims
{
	ONE_H, TWO_H, FOUR_H, FIVE_H, EIGHT_H, ONE_K, TWO_K, FOUR_K, FIVE_K, EIGHT_K, ONE_UP
};


void PointsSprite::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int animation, Texture& spritesheet)
{
	sprite = Sprite::createSprite(glm::ivec2(32, 16), glm::vec2(0.125f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(11);


	sprite->setAnimationSpeed(ONE_H, 1);
	sprite->addKeyframe(ONE_H, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(TWO_H, 1);
	sprite->addKeyframe(TWO_H, glm::vec2(0.125f, 0.0f));

	sprite->setAnimationSpeed(FOUR_H, 1);
	sprite->addKeyframe(FOUR_H, glm::vec2(0.25f, 0.0f));

	sprite->setAnimationSpeed(FIVE_H, 1);
	sprite->addKeyframe(FIVE_H, glm::vec2(0.375f, 0.0f));

	sprite->setAnimationSpeed(EIGHT_H, 1);
	sprite->addKeyframe(EIGHT_H, glm::vec2(0.5f, 0.0f));

	sprite->setAnimationSpeed(ONE_K, 1);
	sprite->addKeyframe(ONE_K, glm::vec2(0.0f, 0.25f));

	sprite->setAnimationSpeed(TWO_K, 1);
	sprite->addKeyframe(TWO_K, glm::vec2(0.125f, 0.25f));

	sprite->setAnimationSpeed(FOUR_K, 1);
	sprite->addKeyframe(FOUR_K, glm::vec2(0.25f, 0.25f));

	sprite->setAnimationSpeed(FIVE_K, 1);
	sprite->addKeyframe(FIVE_K, glm::vec2(0.375f, 0.25f));

	sprite->setAnimationSpeed(EIGHT_K, 1);
	sprite->addKeyframe(EIGHT_K, glm::vec2(0.5f, 0.25f));

	sprite->setAnimationSpeed(ONE_UP, 1);
	sprite->addKeyframe(ONE_UP, glm::vec2(0.0f, 0.5f));

	sprite->changeAnimation(animation);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPoints.x), float(tileMapDispl.y + posPoints.y)));

	inactive = false;
	vy = 1.f;
	startY = posPoints.y;
}

void PointsSprite::setPosition(const glm::vec2 &pos)
{
	posPoints = pos;
	startY = posPoints.y;
}

glm::fvec2 PointsSprite::getPosition()
{
	return posPoints;
}

void PointsSprite::update(int deltaTime)
{
	sprite->update(deltaTime);
	posPoints.y -= vy;

	if (startY - posPoints.y >= 60) {
		inactive = true;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPoints.x), float(tileMapDispl.y + posPoints.y)));
}

void PointsSprite::render()
{
	sprite->render();
}

bool PointsSprite::get_inactive()
{
	return inactive;
}