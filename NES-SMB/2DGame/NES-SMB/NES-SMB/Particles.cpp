#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Particles.h"
#include "Game.h"

enum ParticlesAnims
{
	SPINING_CLOCK, SPINING_ICLOCK
};


void Particles::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spritesheet, float x_vel, float y_vel, int animation)
{
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.5f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);


	sprite->setAnimationSpeed(SPINING_CLOCK, 8);
	sprite->addKeyframe(SPINING_CLOCK, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(SPINING_CLOCK, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(SPINING_CLOCK, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(SPINING_CLOCK, glm::vec2(0.0f, 0.5f));

	sprite->setAnimationSpeed(SPINING_ICLOCK, 8);
	sprite->addKeyframe(SPINING_ICLOCK, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(SPINING_ICLOCK, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(SPINING_ICLOCK, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(SPINING_ICLOCK, glm::vec2(0.5f, 0.5f));

	sprite->changeAnimation(animation);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posParticle.x), float(tileMapDispl.y + posParticle.y)));

	inactive = false;
	vx = x_vel;
	vy = y_vel;
	startY = posParticle.y;
}

void Particles::setPosition(const glm::vec2 &pos) 
{
	posParticle = pos;
	startY = posParticle.y;
}

void Particles::update(int deltaTime)
{
	sprite->update(deltaTime);

	vy -= 0.03f * deltaTime;

	posParticle.y -= vy;
	posParticle.x += vx;

	if (posParticle.y >= 600) {
		inactive = true;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posParticle.x), float(tileMapDispl.y + posParticle.y)));
}


void Particles::render()
{
	sprite->render();
}

bool Particles::get_inactive()
{
	return inactive;
}