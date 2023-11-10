#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define FALL_STEP 5


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_RIGHT, JUMP_LEFT, TURN_LEFT, TURN_RIGHT, STAND_FLAG, RSTAND_FLAG, DOWN_FLAG, DEATH
};

enum TransitionAnims
{
	GROW, SHRINK
};

enum ClampedAnims
{
	CLAMPED_LEFT, CLAMPED_RIGHT, STAR_LEFT, STAR_RIGHT
};

Player::~Player()
{
	if (spriteM != NULL) {
		delete spriteM;
	}
	if (spriteSM != NULL) {
		delete spriteSM;
	}
	if (spriteSuperStM != NULL) {
		delete spriteSuperStM;
	}
	if (spriteSmallStM != NULL) {
		delete spriteSmallStM;
	}
	if (spriteChange != NULL) {
		delete spriteChange;
	}
	if (engine != NULL) {
		delete engine;
	}
}

void Player::reset()
{
	counter = 0;
	invulnerable = false;
	infinite = false;
	dead = false;
	dying = false;
	bJumping = false;
	small_jump = false;
	growing = false;
	shrinking = false;
	Moving = false;
	marg = false;
	apex = false;
	inFlag = false;
	countAnimation = 0;
	finalAnimation = false;
	starMario = false;
	superMario = false;
	spriteSize = glm::ivec2(32, 32);
	vx = 0.f;
	vy = 0.f;
	spriteM->changeAnimation(1);
	spriteM->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	oldPlayer = posPlayer;
	sprite = spriteM;
}


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spritesheetM, Texture &spritesheetSM, Texture &spritesheetSuperStM, Texture &spritesheetSmallStM, Texture &spritesheetChange)
{
	counter = 0;
	engine = irrklang::createIrrKlangDevice();
	invulnerable = false;
	infinite = false;
	dead = false;
	dying = false;
	bJumping = false;
	small_jump = false;
	growing = false;
	shrinking = false;
	Moving = false;
	marg = false;
	apex = false;
	inFlag = false;
	countAnimation = 0;
	finalAnimation = false;
	starMario = false;
	superMario = false;
	spriteSize = glm::ivec2(32, 32);
	vx = 0.f;
	vy = 0.f;

	spriteM = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125f, 0.5f), &spritesheetM, &shaderProgram);
	spriteM->setNumberAnimations(12);

	spriteM->setAnimationSpeed(STAND_LEFT, 8);
	spriteM->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));

	spriteM->setAnimationSpeed(STAND_RIGHT, 8);
	spriteM->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	spriteM->setAnimationSpeed(MOVE_LEFT, 8);
	spriteM->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.5f));
	spriteM->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.5f));
	spriteM->addKeyframe(MOVE_LEFT, glm::vec2(0.375f, 0.5f));

	spriteM->setAnimationSpeed(MOVE_RIGHT, 8);
	spriteM->addKeyframe(MOVE_RIGHT, glm::vec2(0.125, 0.f));
	spriteM->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
	spriteM->addKeyframe(MOVE_RIGHT, glm::vec2(0.375, 0.f));

	spriteM->setAnimationSpeed(JUMP_LEFT, 8);
	spriteM->addKeyframe(JUMP_LEFT, glm::vec2(0.625f, 0.5f));

	spriteM->setAnimationSpeed(JUMP_RIGHT, 8);
	spriteM->addKeyframe(JUMP_RIGHT, glm::vec2(0.625f, 0.f));

	spriteM->setAnimationSpeed(TURN_LEFT, 8);
	spriteM->addKeyframe(TURN_LEFT, glm::vec2(0.5f, 0.5f));

	spriteM->setAnimationSpeed(TURN_RIGHT, 8);
	spriteM->addKeyframe(TURN_RIGHT, glm::vec2(0.5f, 0.f));

	spriteM->setAnimationSpeed(STAND_FLAG, 2);
	spriteM->addKeyframe(STAND_FLAG, glm::vec2(0.875f, 0.f));

	spriteM->setAnimationSpeed(RSTAND_FLAG, 2);
	spriteM->addKeyframe(RSTAND_FLAG, glm::vec2(0.75f, 0.5f));

	spriteM->setAnimationSpeed(DOWN_FLAG, 4);
	spriteM->addKeyframe(DOWN_FLAG, glm::vec2(0.875f, 0.0f));
	spriteM->addKeyframe(DOWN_FLAG, glm::vec2(0.875f, 0.5f));

	spriteM->setAnimationSpeed(DEATH, 8);
	spriteM->addKeyframe(DEATH, glm::vec2(0.75f, 0.f));

	spriteSuperStM = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.125f, 0.25f), &spritesheetSuperStM, &shaderProgram);
	spriteSuperStM->setNumberAnimations(11);

	spriteSuperStM->setAnimationSpeed(STAND_LEFT, 22);
	spriteSuperStM->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));
	spriteSuperStM->addKeyframe(STAND_LEFT, glm::vec2(0.125f, 0.5f));
	spriteSuperStM->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.5f));

	spriteSuperStM->setAnimationSpeed(STAND_RIGHT, 22);
	spriteSuperStM->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
	spriteSuperStM->addKeyframe(STAND_RIGHT, glm::vec2(0.125f, 0.f));
	spriteSuperStM->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

	spriteSuperStM->setAnimationSpeed(MOVE_LEFT, 22);
	spriteSuperStM->addKeyframe(MOVE_LEFT, glm::vec2(0.375f, 0.5f));
	spriteSuperStM->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.5f));
	spriteSuperStM->addKeyframe(MOVE_LEFT, glm::vec2(0.625f, 0.5f));

	spriteSuperStM->setAnimationSpeed(MOVE_RIGHT, 22);
	spriteSuperStM->addKeyframe(MOVE_RIGHT, glm::vec2(0.375, 0.f));
	spriteSuperStM->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.f));
	spriteSuperStM->addKeyframe(MOVE_RIGHT, glm::vec2(0.625, 0.f));

	spriteSuperStM->setAnimationSpeed(JUMP_LEFT, 22);
	spriteSuperStM->addKeyframe(JUMP_LEFT, glm::vec2(0.375f, 0.75f));
	spriteSuperStM->addKeyframe(JUMP_LEFT, glm::vec2(0.5f, 0.75f));
	spriteSuperStM->addKeyframe(JUMP_LEFT, glm::vec2(0.625f, 0.75f));

	spriteSuperStM->setAnimationSpeed(JUMP_RIGHT, 22);
	spriteSuperStM->addKeyframe(JUMP_RIGHT, glm::vec2(0.375f, 0.25f));
	spriteSuperStM->addKeyframe(JUMP_RIGHT, glm::vec2(0.5f, 0.25f));
	spriteSuperStM->addKeyframe(JUMP_RIGHT, glm::vec2(0.625f, 0.25f));

	spriteSuperStM->setAnimationSpeed(TURN_LEFT, 22);
	spriteSuperStM->addKeyframe(TURN_LEFT, glm::vec2(0.f, 0.75f));
	spriteSuperStM->addKeyframe(TURN_LEFT, glm::vec2(0.125f, 0.75f));
	spriteSuperStM->addKeyframe(TURN_LEFT, glm::vec2(0.25f, 0.75f));

	spriteSuperStM->setAnimationSpeed(TURN_RIGHT, 22);
	spriteSuperStM->addKeyframe(TURN_RIGHT, glm::vec2(0.f, 0.25f));
	spriteSuperStM->addKeyframe(TURN_RIGHT, glm::vec2(0.125f, 0.25f));
	spriteSuperStM->addKeyframe(TURN_RIGHT, glm::vec2(0.25f, 0.25f));

	spriteSuperStM->setAnimationSpeed(STAND_FLAG, 22);
	spriteSuperStM->addKeyframe(STAND_FLAG, glm::vec2(0.75f, 0.f));
	spriteSuperStM->addKeyframe(STAND_FLAG, glm::vec2(0.875f, 0.f));
	spriteSuperStM->addKeyframe(STAND_FLAG, glm::vec2(0.75f, 0.25f));

	spriteSuperStM->setAnimationSpeed(RSTAND_FLAG, 22);
	spriteSuperStM->addKeyframe(RSTAND_FLAG, glm::vec2(0.875f, 0.25f));
	spriteSuperStM->addKeyframe(RSTAND_FLAG, glm::vec2(0.75f, 0.5f));
	spriteSuperStM->addKeyframe(RSTAND_FLAG, glm::vec2(0.875f, 0.5f));

	spriteSuperStM->setAnimationSpeed(DOWN_FLAG, 18);
	spriteSuperStM->addKeyframe(DOWN_FLAG, glm::vec2(0.75f, 0.f));
	spriteSuperStM->addKeyframe(DOWN_FLAG, glm::vec2(0.875f, 0.f));
	spriteSuperStM->addKeyframe(DOWN_FLAG, glm::vec2(0.75f, 0.25f));
	spriteSuperStM->addKeyframe(DOWN_FLAG, glm::vec2(0.75f, 0.75f));
	spriteSuperStM->addKeyframe(DOWN_FLAG, glm::vec2(0.875f, 0.75f));

	spriteSmallStM = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125f, 0.125f), &spritesheetSmallStM, &shaderProgram);
	spriteSmallStM->setNumberAnimations(11);

	spriteSmallStM->setAnimationSpeed(STAND_LEFT, 22);
	spriteSmallStM->addKeyframe(STAND_LEFT, glm::vec2(0.625f, 0.0f));
	spriteSmallStM->addKeyframe(STAND_LEFT, glm::vec2(0.5f, 0.0f));
	spriteSmallStM->addKeyframe(STAND_LEFT, glm::vec2(0.375f, 0.0f));

	spriteSmallStM->setAnimationSpeed(STAND_RIGHT, 22);
	spriteSmallStM->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
	spriteSmallStM->addKeyframe(STAND_RIGHT, glm::vec2(0.125f, 0.f));
	spriteSmallStM->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

	spriteSmallStM->setAnimationSpeed(MOVE_LEFT, 22);
	spriteSmallStM->addKeyframe(MOVE_LEFT, glm::vec2(0.625f, 0.125f));
	spriteSmallStM->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.125f));
	spriteSmallStM->addKeyframe(MOVE_LEFT, glm::vec2(0.375f, 0.125f));

	spriteSmallStM->setAnimationSpeed(MOVE_RIGHT, 22);
	spriteSmallStM->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.125f));
	spriteSmallStM->addKeyframe(MOVE_RIGHT, glm::vec2(0.125f, 0.125f));
	spriteSmallStM->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.125f));

	spriteSmallStM->setAnimationSpeed(JUMP_LEFT, 22);
	spriteSmallStM->addKeyframe(JUMP_LEFT, glm::vec2(0.625f, 0.625f));
	spriteSmallStM->addKeyframe(JUMP_LEFT, glm::vec2(0.5f, 0.625f));
	spriteSmallStM->addKeyframe(JUMP_LEFT, glm::vec2(0.375f, 0.625f));

	spriteSmallStM->setAnimationSpeed(JUMP_RIGHT, 22);
	spriteSmallStM->addKeyframe(JUMP_RIGHT, glm::vec2(0.0f, 0.625f));
	spriteSmallStM->addKeyframe(JUMP_RIGHT, glm::vec2(0.125f, 0.625f));
	spriteSmallStM->addKeyframe(JUMP_RIGHT, glm::vec2(0.25f, 0.625f));

	spriteSmallStM->setAnimationSpeed(TURN_LEFT, 22);
	spriteSmallStM->addKeyframe(TURN_LEFT, glm::vec2(0.625f, 0.25f));
	spriteSmallStM->addKeyframe(TURN_LEFT, glm::vec2(0.5f, 0.25f));
	spriteSmallStM->addKeyframe(TURN_LEFT, glm::vec2(0.375f, 0.25f));

	spriteSmallStM->setAnimationSpeed(TURN_RIGHT, 22);
	spriteSmallStM->addKeyframe(TURN_RIGHT, glm::vec2(0.f, 0.25f));
	spriteSmallStM->addKeyframe(TURN_RIGHT, glm::vec2(0.125f, 0.25f));
	spriteSmallStM->addKeyframe(TURN_RIGHT, glm::vec2(0.25f, 0.25f));

	spriteSmallStM->setAnimationSpeed(STAND_FLAG, 22);
	spriteSmallStM->addKeyframe(STAND_FLAG, glm::vec2(0.0f, 0.375f));
	spriteSmallStM->addKeyframe(STAND_FLAG, glm::vec2(0.125f, 0.375f));
	spriteSmallStM->addKeyframe(STAND_FLAG, glm::vec2(0.25f, 0.375f));

	spriteSmallStM->setAnimationSpeed(RSTAND_FLAG, 22);
	spriteSmallStM->addKeyframe(RSTAND_FLAG, glm::vec2(0.625f, 0.375f));
	spriteSmallStM->addKeyframe(RSTAND_FLAG, glm::vec2(0.5f, 0.375f));
	spriteSmallStM->addKeyframe(RSTAND_FLAG, glm::vec2(0.375f, 0.375f));

	spriteSmallStM->setAnimationSpeed(DOWN_FLAG, 18);
	spriteSmallStM->addKeyframe(DOWN_FLAG, glm::vec2(0.0f, 0.375f));
	spriteSmallStM->addKeyframe(DOWN_FLAG, glm::vec2(0.125f, 0.375f));
	spriteSmallStM->addKeyframe(DOWN_FLAG, glm::vec2(0.25f, 0.375f));
	spriteSmallStM->addKeyframe(DOWN_FLAG, glm::vec2(0.0f, 0.5f));
	spriteSmallStM->addKeyframe(DOWN_FLAG, glm::vec2(0.125f, 0.5f));
	spriteSmallStM->addKeyframe(DOWN_FLAG, glm::vec2(0.25f, 0.5f));

	spriteSM = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.125f, 0.5f), &spritesheetSM, &shaderProgram);
	spriteSM->setNumberAnimations(12);

	spriteSM->setAnimationSpeed(STAND_LEFT, 8);
	spriteSM->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));

	spriteSM->setAnimationSpeed(STAND_RIGHT, 8);
	spriteSM->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	spriteSM->setAnimationSpeed(MOVE_LEFT, 8);
	spriteSM->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.5f));
	spriteSM->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.5f));
	spriteSM->addKeyframe(MOVE_LEFT, glm::vec2(0.375f, 0.5f));

	spriteSM->setAnimationSpeed(MOVE_RIGHT, 8);
	spriteSM->addKeyframe(MOVE_RIGHT, glm::vec2(0.125, 0.f));
	spriteSM->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
	spriteSM->addKeyframe(MOVE_RIGHT, glm::vec2(0.375, 0.f));

	spriteSM->setAnimationSpeed(JUMP_LEFT, 8);
	spriteSM->addKeyframe(JUMP_LEFT, glm::vec2(0.625f, 0.5f));

	spriteSM->setAnimationSpeed(JUMP_RIGHT, 8);
	spriteSM->addKeyframe(JUMP_RIGHT, glm::vec2(0.625f, 0.f));

	spriteSM->setAnimationSpeed(TURN_LEFT, 8);
	spriteSM->addKeyframe(TURN_LEFT, glm::vec2(0.5f, 0.5f));

	spriteSM->setAnimationSpeed(TURN_RIGHT, 8);
	spriteSM->addKeyframe(TURN_RIGHT, glm::vec2(0.5f, 0.f));

	spriteSM->setAnimationSpeed(DEATH, 8);
	spriteSM->addKeyframe(DEATH, glm::vec2(0.75f, 0.f));

	spriteSM->setAnimationSpeed(STAND_FLAG, 2);
	spriteSM->addKeyframe(STAND_FLAG, glm::vec2(0.875f, 0.f));

	spriteSM->setAnimationSpeed(RSTAND_FLAG, 2);
	spriteSM->addKeyframe(RSTAND_FLAG, glm::vec2(0.75f, 0.5f));

	spriteSM->setAnimationSpeed(DOWN_FLAG, 4);
	spriteSM->addKeyframe(DOWN_FLAG, glm::vec2(0.875f, 0.0f));
	spriteSM->addKeyframe(DOWN_FLAG, glm::vec2(0.875f, 0.5f));

	spriteM->changeAnimation(1);

	spriteChange = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.125f, 1.f), &spritesheetChange, &shaderProgram);
	spriteChange->setNumberAnimations(2);

	spriteChange->setAnimationSpeed(GROW, 22);
	spriteChange->addKeyframe(GROW, glm::vec2(0.f, 0.f));
	spriteChange->addKeyframe(GROW, glm::vec2(0.125f, 0.f));
	spriteChange->addKeyframe(GROW, glm::vec2(0.f, 0.f));
	spriteChange->addKeyframe(GROW, glm::vec2(0.125f, 0.f));
	spriteChange->addKeyframe(GROW, glm::vec2(0.f, 0.f));
	spriteChange->addKeyframe(GROW, glm::vec2(0.125f, 0.f));
	spriteChange->addKeyframe(GROW, glm::vec2(0.f, 0.f));
	spriteChange->addKeyframe(GROW, glm::vec2(0.125f, 0.f));
	spriteChange->addKeyframe(GROW, glm::vec2(0.25f, 0.f));
	spriteChange->addKeyframe(GROW, glm::vec2(0.f, 0.f));
	spriteChange->addKeyframe(GROW, glm::vec2(0.125f, 0.f));
	spriteChange->addKeyframe(GROW, glm::vec2(0.25f, 0.f));

	spriteChange->setAnimationSpeed(SHRINK, 22);
	spriteChange->addKeyframe(SHRINK, glm::vec2(0.375f, 0.f));
	spriteChange->addKeyframe(SHRINK, glm::vec2(0.375f, 0.f));
	spriteChange->addKeyframe(SHRINK, glm::vec2(0.5f, 0.f));
	spriteChange->addKeyframe(SHRINK, glm::vec2(0.375f, 0.f));
	spriteChange->addKeyframe(SHRINK, glm::vec2(0.5f, 0.f));
	spriteChange->addKeyframe(SHRINK, glm::vec2(0.625f, 0.f));
	spriteChange->addKeyframe(SHRINK, glm::vec2(0.5f, 0.f));
	spriteChange->addKeyframe(SHRINK, glm::vec2(0.625f, 0.f));
	spriteChange->addKeyframe(SHRINK, glm::vec2(0.75f, 0.f));

	spriteChange->changeAnimation(GROW);	

	tileMapDispl = tileMapPos;
	spriteM->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	oldPlayer = posPlayer;
	sprite = spriteM;
}

void Player::setAnimationSpeed()
{
	spriteSuperStM->setAnimationSpeed(STAND_LEFT, 8);
	spriteSmallStM->setAnimationSpeed(STAND_LEFT, 8);

	spriteSuperStM->setAnimationSpeed(STAND_RIGHT, 8);
	spriteSmallStM->setAnimationSpeed(STAND_RIGHT, 8);

	spriteSuperStM->setAnimationSpeed(MOVE_LEFT, 8);
	spriteSmallStM->setAnimationSpeed(MOVE_LEFT, 8);

	spriteSuperStM->setAnimationSpeed(MOVE_RIGHT, 8);
	spriteSmallStM->setAnimationSpeed(MOVE_RIGHT, 8);

	spriteSuperStM->setAnimationSpeed(JUMP_LEFT, 8);
	spriteSmallStM->setAnimationSpeed(JUMP_LEFT, 8);

	spriteSuperStM->setAnimationSpeed(JUMP_RIGHT, 8);
	spriteSmallStM->setAnimationSpeed(JUMP_RIGHT, 8);

	spriteSuperStM->setAnimationSpeed(TURN_LEFT, 8);
	spriteSmallStM->setAnimationSpeed(TURN_LEFT, 8);

	spriteSuperStM->setAnimationSpeed(TURN_RIGHT, 8);
	spriteSmallStM->setAnimationSpeed(TURN_RIGHT, 8);

	spriteSuperStM->setAnimationSpeed(STAND_FLAG, 8);
	spriteSmallStM->setAnimationSpeed(STAND_FLAG, 8);

	spriteSuperStM->setAnimationSpeed(RSTAND_FLAG, 8);
	spriteSmallStM->setAnimationSpeed(RSTAND_FLAG, 8);

	spriteSuperStM->setAnimationSpeed(DOWN_FLAG, 8);
	spriteSmallStM->setAnimationSpeed(DOWN_FLAG, 8);
}

void Player::setBackAnimationSpeed()
{
	spriteSuperStM->setAnimationSpeed(STAND_LEFT, 22);
	spriteSmallStM->setAnimationSpeed(STAND_LEFT, 22);

	spriteSuperStM->setAnimationSpeed(STAND_RIGHT, 22);
	spriteSmallStM->setAnimationSpeed(STAND_RIGHT, 22);

	spriteSuperStM->setAnimationSpeed(MOVE_LEFT, 22);
	spriteSmallStM->setAnimationSpeed(MOVE_LEFT, 22);

	spriteSuperStM->setAnimationSpeed(MOVE_RIGHT, 22);
	spriteSmallStM->setAnimationSpeed(MOVE_RIGHT, 22);

	spriteSuperStM->setAnimationSpeed(JUMP_LEFT, 22);
	spriteSmallStM->setAnimationSpeed(JUMP_LEFT, 22);

	spriteSuperStM->setAnimationSpeed(JUMP_RIGHT, 22);
	spriteSmallStM->setAnimationSpeed(JUMP_RIGHT, 22);

	spriteSuperStM->setAnimationSpeed(TURN_LEFT, 22);
	spriteSmallStM->setAnimationSpeed(TURN_LEFT, 22);

	spriteSuperStM->setAnimationSpeed(TURN_RIGHT, 22);
	spriteSmallStM->setAnimationSpeed(TURN_RIGHT, 22);

	spriteSuperStM->setAnimationSpeed(STAND_FLAG, 22);
	spriteSmallStM->setAnimationSpeed(STAND_FLAG, 22);

	spriteSuperStM->setAnimationSpeed(RSTAND_FLAG, 22);
	spriteSmallStM->setAnimationSpeed(RSTAND_FLAG, 22);

	spriteSuperStM->setAnimationSpeed(DOWN_FLAG, 18);
	spriteSmallStM->setAnimationSpeed(DOWN_FLAG, 18);
}

void Player::flagTreatment(int deltaTime)
{
	if (!(engine->isCurrentlyPlaying("sounds/flagpoleOriginal.wav"))) {
		engine->play2D("sounds/flagpoleOriginal.wav", false, false, true);
	}
	posPlayer.x = 197 * 32 + 18;
	if (sprite->animation() != DOWN_FLAG)
		sprite->changeAnimation(DOWN_FLAG);

	int maxDown = 11 * 32;
	if (spriteSize.y == 64)
		maxDown = 10 * 32;

	if (posPlayer.y < maxDown) {
		//sound

		posPlayer.y = posPlayer.y + 4;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		counter+=deltaTime;
	}
	else {
		counter += deltaTime;
		vx = 2.f;
		vy = 0.f;
		sprite->changeAnimation(RSTAND_FLAG);
		posPlayer.x = 198 * 32 + 11;
		posPlayer.y = maxDown;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		if (counter > 1100) {
			inFlag = false;
			bJumping = false;
			finalAnimation = true;
		}
	}
}

void Player::getOut(int deltaTime) {

	if (!engine->isCurrentlyPlaying("sounds/stage_clear.wav")) {
		engine->play2D("sounds/stage_clear.wav", false, false);
	}

	sprite->update(deltaTime);
	if (sprite->animation() != MOVE_RIGHT) {
		sprite->setAnimationSpeed(MOVE_RIGHT, 6);
		sprite->changeAnimation(MOVE_RIGHT);
	}
	if (posPlayer.x <= 6528) {
		posPlayer.x += int(vx);
		posPlayer.y += FALL_STEP;
		map->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y);
	}

}

void Player::setMarioSprite() {
	superMario = false;
	starMario = false;
	int animation = sprite->animation();
	sprite = spriteM;
	if (sprite->animation() != animation) {
		sprite->changeAnimation(animation);
	}

	if (spriteSize.y == 64) {
		posPlayer.y = posPlayer.y + 32;
		spriteSize = glm::ivec2(32, 32);
	}

	spriteSize = glm::ivec2(32, 32);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::setSuperMarioSprite() {
	/*
	if (!starMario && !superMario) {
		if (!(engine->isCurrentlyPlaying("sounds/getPowerup.wav"))) {
			engine->play2D("sounds/getPowerup.wav", false, false, true);
		}
	}
	*/

	int animation = sprite->animation();
	superMario = true;
	if (!starMario) { //smallMario
		sprite = spriteSM;
		if (sprite->animation() != animation) {
			sprite->changeAnimation(animation);
		}
	}
	else { //smallStarMario
		sprite = spriteSuperStM;
		if (sprite->animation() != animation) {
			sprite->changeAnimation(animation);
		}
	}
	if (spriteSize.y == 32) {
		posPlayer.y = posPlayer.y - 32;
		spriteSize = glm::ivec2(32, 64);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::setStarMarioSprite() {
	/*
	if (!starMario && !superMario) {
		if (!(engine->isCurrentlyPlaying("sounds/getPowerup.wav"))) {
			engine->play2D("sounds/getPowerup.wav", false, false, true);
		}
	}
	*/


	int animation = sprite->animation();
	starMario = true;

	if (superMario) {
		sprite = spriteSuperStM;
	}
	else {
		sprite = spriteSmallStM;
	}
	if (sprite->animation() != animation) {
		sprite->changeAnimation(animation);
	}
	/*if (spriteSize.y == 32) {
	posPlayer.y = posPlayer.y - 32;
	spriteSize = glm::ivec2(32, 64);
	}
	spriteSize = glm::ivec2(32, 64);*/

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::set_Growing()
{
	sprite = spriteChange;
	posPlayer.y -= 32;
	sprite->changeAnimation(GROW);
	growing = true;
	//sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

bool Player::get_Growing()
{
	return growing;
}

void Player::set_Shrinking()
{
	sprite = spriteChange;
	//posPlayer.y -= 32;
	sprite->changeAnimation(SHRINK);
	shrinking = true;
	invulnerable = true;
	//sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

bool Player::get_Shrinking()
{
	return shrinking;
}

void Player::unset_Invulnerable()
{
	invulnerable = false;
}

bool Player::get_Invulnerable()
{
	return invulnerable;
}

void Player::update(int deltaTime)
{
	if (dying) {
		if ((death_jump - posPlayer.y) >= 50) {
			if (vy > -5.f) {
				vy -= 0.08f * deltaTime;
			}
		}
		int dvy = int(vy);
		posPlayer.y -= dvy;
		if ((posPlayer.y > 512) && (vy < 0.f)) {
			dying = false;
			dead = true;
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}

	else if (growing) {
		sprite = spriteChange;
		if (sprite->animation() != GROW)
			sprite->changeAnimation(GROW);
		sprite->update(deltaTime);
		if (sprite->get_currentKeyframe() == 11) {
			posPlayer.y += 32;
			growing = false;
			setSuperMarioSprite();
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}

	else if (shrinking) {
		sprite = spriteChange;
		if (sprite->animation() != SHRINK)
			sprite->changeAnimation(SHRINK);
		sprite->update(deltaTime);
		if (sprite->get_currentKeyframe() == 8) {
			//posPlayer.y -= 32;
			shrinking = false;
			setMarioSprite();
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}

	else if (finalAnimation) {
		getOut(deltaTime);
	}

	else {

		if (Game::instance().getKey('n') && (superMario || starMario))
		{
			infinite = false;
			setMarioSprite();
		}
		else if (Game::instance().getKey('m') && (!superMario || starMario))
		{
			infinite = false;
			setSuperMarioSprite();
		}
		else if (Game::instance().getKey('g') && !starMario)
		{
			infinite = true;
			setStarMarioSprite();
		}

		sprite->update(deltaTime);
		oldPlayer = posPlayer;

		if (inFlag && !finalAnimation) {
			flagTreatment(deltaTime);
		}

		else {

			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				if (vx < -4.50f)
					sprite->setAnimationSpeed(MOVE_LEFT, 20);
				else if (vx < -3.50f)
					sprite->setAnimationSpeed(MOVE_LEFT, 16);
				else if (vx < -2.90f)
					sprite->setAnimationSpeed(MOVE_LEFT, 14);
				else
					sprite->setAnimationSpeed(MOVE_LEFT, 8);
				if (vx <= 0.f) {
					if (sprite->animation() != MOVE_LEFT && !bJumping && vx < 0.f)
						sprite->changeAnimation(MOVE_LEFT);
				}

				else {
					//posPlayer.x += int(vx);
					if (!bJumping && vx > 0.f && sprite->animation() != TURN_LEFT)
						sprite->changeAnimation(TURN_LEFT);
				}

				if ((vx > -3.f) || (Game::instance().getSpecialKey(112) && (vx > -5.f))) {
					if (bJumping) {
						if (!(Game::instance().getSpecialKey(112))) {
							if (vx > 3.0)
								vx -= 0.015f * deltaTime;
							else
								vx -= 0.005f * deltaTime;
						}
						else {
							if (vx > 3.0)
								vx -= 0.015f * deltaTime;
							else
								vx -= 0.005f * deltaTime;
						}
					}
					else {
						if (!(Game::instance().getSpecialKey(112))) {
							if (vx > 3.0)
								vx -= 0.03f * deltaTime;
							else
								vx -= 0.01f * deltaTime;
						}
						else {
							if (vx > 3.0)
								vx -= 0.03f * deltaTime;
							else
								vx -= 0.01f * deltaTime;
						}
					}
				}

				int dv = int(vx);
				posPlayer.x += dv;
			}

			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				if (vx > 4.50f)
					sprite->setAnimationSpeed(MOVE_RIGHT, 20);
				else if (vx > 3.50f)
					sprite->setAnimationSpeed(MOVE_RIGHT, 16);
				else if (vx > 2.90f)
					sprite->setAnimationSpeed(MOVE_RIGHT, 14);
				else
					sprite->setAnimationSpeed(MOVE_RIGHT, 8);
				if (vx >= 0.f) {
					if (sprite->animation() != MOVE_RIGHT && !bJumping && vx > 0.f)
						sprite->changeAnimation(MOVE_RIGHT);
				}
				else {
					//posPlayer.x += int(vx);
					if (!bJumping && vx < 0.f && sprite->animation() != TURN_RIGHT)
						sprite->changeAnimation(TURN_RIGHT);
				}

				if ((vx < 3.f) || (Game::instance().getSpecialKey(112) && (vx < 5.f))) {
					if (bJumping) {
						if (!(Game::instance().getSpecialKey(112))) {
							if (vx < -3.0)
								vx += 0.015f * deltaTime;
							else
								vx += 0.015f * deltaTime;
						}
						else {
							if (vx < -3.0)
								vx += 0.005f * deltaTime;
							else
								vx += 0.005f * deltaTime;
						}
					}
					else {
						if (!(Game::instance().getSpecialKey(112))) {
							if (vx < -3.0)
								vx += 0.03f * deltaTime;
							else
								vx += 0.01f * deltaTime;
						}
						else {
							if (vx < -3.0)
								vx += 0.03f * deltaTime;
							else
								vx += 0.01f * deltaTime;
						}
					}
				}

				int dv = int(vx);
				posPlayer.x += dv;
			}

			else
			{
				posPlayer.x += int(vx);
				if (vx < 2.0f)
					sprite->setAnimationSpeed(MOVE_RIGHT, 6);
				if (vx < 1.0f)
					sprite->setAnimationSpeed(MOVE_RIGHT, 4);
				if (vx > -2.0f)
					sprite->setAnimationSpeed(MOVE_LEFT, 6);
				if (vx > -1.0f)
					sprite->setAnimationSpeed(MOVE_LEFT, 4);
				if ((sprite->animation() == MOVE_LEFT) && vx == 0.0f)
					sprite->changeAnimation(STAND_LEFT);
				else if ((sprite->animation() == MOVE_RIGHT) && vx == 0.0f)
					sprite->changeAnimation(STAND_RIGHT);
			}

			//
			if (vx == 0.f && sprite->animation() == TURN_LEFT) {
				sprite->changeAnimation(STAND_LEFT);
			}
			else if (vx == 0.f && sprite->animation() == TURN_RIGHT) {
				sprite->changeAnimation(STAND_RIGHT);
			}
			//

			if (std::abs(vx) > 0.01f) {
				// Simulate friction by reducing the velocity
				if (vx > 0.0f) {
					if (bJumping) {
						vx -= 0.001f * deltaTime;
					}
					else
						vx -= 0.004f * deltaTime;
					if (vx < 0.0f) {
						vx = 0.0f;
					}
				}
				else if (vx < 0.0f) {
					if (bJumping) {
						vx += 0.001f * deltaTime;
					}
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
				if (posPlayer.y > 0) {
					if (map->collisionMoveRight(posPlayer, spriteSize, &posPlayer.x) && !bJumping) {
						if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) {
							vx = 0.0f;
							if (sprite->animation() != STAND_LEFT)
								sprite->changeAnimation(STAND_LEFT);
						}
						else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT) {
							vx = 0.0f;
							if (sprite->animation() != STAND_RIGHT)
								sprite->changeAnimation(STAND_RIGHT);

						}
					}
				}
			}

			else {
				if (posPlayer.y > 0) {
					if (map->collisionMoveLeft(posPlayer, spriteSize, &posPlayer.x, marg, ic) && !bJumping) {
						if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) {
							vx = 0.0f;
							if (sprite->animation() != STAND_LEFT)
								sprite->changeAnimation(STAND_LEFT);
						}
						else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT) {
							vx = 0.0f;
							if (sprite->animation() != STAND_RIGHT)
								sprite->changeAnimation(STAND_RIGHT);

						}
					}
				}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////

			if (small_jump) {
				if ((start_small_jump - posPlayer.y) >= 30) {
					if (vy > -5.f) {
						vy -= 0.1f * deltaTime;
					}
				}
				int dv = int(vy);
				posPlayer.y -= dv;
				if (vy > 0.f)
					small_jump = !map->collisionMoveUp(posPlayer, spriteSize, &posPlayer.y);
				else
					small_jump = !map->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y);
			}

			else {
				if (bJumping)
				{
					if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT || sprite->animation() == TURN_LEFT)
						sprite->changeAnimation(JUMP_LEFT);
					else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT || sprite->animation() == TURN_RIGHT)
						sprite->changeAnimation(JUMP_RIGHT);

					if (Game::instance().getKey(32)) {

						if (vy > 0.f) {
							if ((superMario || starMario)) {
								if (!(engine->isCurrentlyPlaying("sounds/jump_super.wav"))) {
									engine->play2D("sounds/jump_super.wav", false, false, true);
								}
							}
							else if (!(engine->isCurrentlyPlaying("sounds/jump_small.wav"))) {
								engine->play2D("sounds/jump_small.wav", false, false, true);
							}
						}

						if (!apex) {
							if (startY - posPlayer.y <= 64) {
								vy = 7.0f;
							}
							else if (startY - posPlayer.y <= 100) {
								vy = 5.0f;
							}
							else if (startY - posPlayer.y <= 115) {
								vy = 4.0f;
							}
							else if (startY - posPlayer.y <= 116) {
								vy = 1.0f;
							}
							else if (startY - posPlayer.y >= 130)
								apex = true;

						}
						else if (apex) {
							if (vy > -6.f)
								vy -= 0.08f * deltaTime;
						}

						int dv = int(vy);
						posPlayer.y -= dv;

						if (vy > 0.f) {
							if (posPlayer.y > 0) {
								bJumping = !map->collisionMoveUp(posPlayer, spriteSize, &posPlayer.y);
							}
						}
						else
							bJumping = !map->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y);
					}
					else {
						apex = true;
						if (abs(startY - posPlayer.y) >= 54)
							if (vy > -6.f)
								vy -= 0.08f * deltaTime;
						int dv = int(vy);
						posPlayer.y -= dv;
						if (vy > 0.f) {
							if (posPlayer.y > 0) {
								bJumping = !map->collisionMoveUp(posPlayer, spriteSize, &posPlayer.y);
							}
						}
						else
							bJumping = !map->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y);
					}
				}
				else
				{
					apex = false;
					//vy = 0.f;
					posPlayer.y += FALL_STEP;
					if (map->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y))
					{
						if (sprite->animation() == JUMP_LEFT)
							sprite->changeAnimation(MOVE_LEFT);
						else if (sprite->animation() == JUMP_RIGHT)
							sprite->changeAnimation(MOVE_RIGHT);

						if (Game::instance().getKey(32))
						{
							bJumping = true;
							jumpAngle = 0;
							startY = posPlayer.y;
						}
					}
				}
			}

			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		}
	}
	// MIRAR SI HACE FALTA
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
	setMarioSprite();
	sprite->changeAnimation(DEATH);
	vy = 5.f;
	vx = 0.f;
	death_jump = posPlayer.y;
	dying = true;
}

void Player::setInFlag() {
	inFlag = true;
}

bool Player::getFinalAnimation() {
	return finalAnimation;
}

glm::ivec2 Player::getMarioSpriteSize() {
	return spriteSize;
}

bool Player::isStarMario() {
	return starMario;
}

bool Player::isSuperMario() {
	return superMario;
}

void Player::unsetStarMario() {
	starMario = false;
}

void Player::hit() {
	setMarioSprite();
}

void Player::set_small_jump() {
	if (!starMario) {
		vy = 5.0f;
		start_small_jump = posPlayer.y;
		small_jump = true;
	}
}

bool Player::killed()
{
	return dead;
}

bool Player::being_killed()
{
	return dying;
}

bool Player::is_infinite()
{
	return infinite;
}

void Player::silence()
{
	engine->stopAllSounds();
}

bool Player::get_inFlag()
{
	return inFlag;
}

bool Player::is_final_song() 
{
	return engine->isCurrentlyPlaying("sounds/stage_clear.wav");
}