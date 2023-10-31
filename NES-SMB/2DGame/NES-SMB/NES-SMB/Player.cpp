#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_RIGHT, JUMP_LEFT, TURN_LEFT, TURN_RIGHT, STAND_FLAG, RSTAND_FLAG, DOWN_FLAG, DEATH
};

void Player::reset()
{
	bJumping = false;
	small_jump = false;
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


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	engine = irrklang::createIrrKlangDevice();
	bJumping = false;
	small_jump = false;
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
	spritesheetM.loadFromFile("images/small_mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
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

	spritesheetSuperStM.loadFromFile("images/superStarMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteSuperStM = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.125f, 0.25f), &spritesheetSuperStM, &shaderProgram);
	spriteSuperStM->setNumberAnimations(11);

	spriteSuperStM->setAnimationSpeed(STAND_LEFT, 16);
	spriteSuperStM->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));
	spriteSuperStM->addKeyframe(STAND_LEFT, glm::vec2(0.125f, 0.5f));
	spriteSuperStM->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.5f));

	spriteSuperStM->setAnimationSpeed(STAND_RIGHT, 16);
	spriteSuperStM->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
	spriteSuperStM->addKeyframe(STAND_RIGHT, glm::vec2(0.125f, 0.f));
	spriteSuperStM->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

	spriteSuperStM->setAnimationSpeed(MOVE_LEFT, 16);
	spriteSuperStM->addKeyframe(MOVE_LEFT, glm::vec2(0.375f, 0.5f));
	spriteSuperStM->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.5f));
	spriteSuperStM->addKeyframe(MOVE_LEFT, glm::vec2(0.625f, 0.5f));

	spriteSuperStM->setAnimationSpeed(MOVE_RIGHT, 16);
	spriteSuperStM->addKeyframe(MOVE_RIGHT, glm::vec2(0.375, 0.f));
	spriteSuperStM->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.f));
	spriteSuperStM->addKeyframe(MOVE_RIGHT, glm::vec2(0.625, 0.f));

	spriteSuperStM->setAnimationSpeed(JUMP_LEFT, 16);
	spriteSuperStM->addKeyframe(JUMP_LEFT, glm::vec2(0.375f, 0.75f));
	spriteSuperStM->addKeyframe(JUMP_LEFT, glm::vec2(0.5f, 0.75f));
	spriteSuperStM->addKeyframe(JUMP_LEFT, glm::vec2(0.625f, 0.75f));

	spriteSuperStM->setAnimationSpeed(JUMP_RIGHT, 16);
	spriteSuperStM->addKeyframe(JUMP_RIGHT, glm::vec2(0.375f, 0.25f));
	spriteSuperStM->addKeyframe(JUMP_RIGHT, glm::vec2(0.5f, 0.25f));
	spriteSuperStM->addKeyframe(JUMP_RIGHT, glm::vec2(0.625f, 0.25f));

	spriteSuperStM->setAnimationSpeed(TURN_LEFT, 16);
	spriteSuperStM->addKeyframe(TURN_LEFT, glm::vec2(0.f, 0.75f));
	spriteSuperStM->addKeyframe(TURN_LEFT, glm::vec2(0.125f, 0.75f));
	spriteSuperStM->addKeyframe(TURN_LEFT, glm::vec2(0.25f, 0.75f));

	spriteSuperStM->setAnimationSpeed(TURN_RIGHT, 16);
	spriteSuperStM->addKeyframe(TURN_RIGHT, glm::vec2(0.f, 0.25f));
	spriteSuperStM->addKeyframe(TURN_RIGHT, glm::vec2(0.125f, 0.25f));
	spriteSuperStM->addKeyframe(TURN_RIGHT, glm::vec2(0.25f, 0.25f));

	spriteSuperStM->setAnimationSpeed(STAND_FLAG, 16);
	spriteSuperStM->addKeyframe(STAND_FLAG, glm::vec2(0.75f, 0.f));
	spriteSuperStM->addKeyframe(STAND_FLAG, glm::vec2(0.875f, 0.f));
	spriteSuperStM->addKeyframe(STAND_FLAG, glm::vec2(0.75f, 0.25f));

	spriteSuperStM->setAnimationSpeed(RSTAND_FLAG, 16);
	spriteSuperStM->addKeyframe(RSTAND_FLAG, glm::vec2(0.875f, 0.25f));
	spriteSuperStM->addKeyframe(RSTAND_FLAG, glm::vec2(0.75f, 0.5f));
	spriteSuperStM->addKeyframe(RSTAND_FLAG, glm::vec2(0.875f, 0.5f));

	spriteSuperStM->setAnimationSpeed(DOWN_FLAG, 12);
	spriteSuperStM->addKeyframe(DOWN_FLAG, glm::vec2(0.75f, 0.f));
	spriteSuperStM->addKeyframe(DOWN_FLAG, glm::vec2(0.875f, 0.f));
	spriteSuperStM->addKeyframe(DOWN_FLAG, glm::vec2(0.75f, 0.25f));
	spriteSuperStM->addKeyframe(DOWN_FLAG, glm::vec2(0.75f, 0.75f));
	spriteSuperStM->addKeyframe(DOWN_FLAG, glm::vec2(0.875f, 0.75f));

	spritesheetSmallStM.loadFromFile("images/smallStarMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteSmallStM = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125f, 0.125f), &spritesheetSmallStM, &shaderProgram);
	spriteSmallStM->setNumberAnimations(11);

	spriteSmallStM->setAnimationSpeed(STAND_LEFT, 16);
	spriteSmallStM->addKeyframe(STAND_LEFT, glm::vec2(0.625f, 0.0f));
	spriteSmallStM->addKeyframe(STAND_LEFT, glm::vec2(0.5f, 0.0f));
	spriteSmallStM->addKeyframe(STAND_LEFT, glm::vec2(0.375f, 0.0f));

	spriteSmallStM->setAnimationSpeed(STAND_RIGHT, 16);
	spriteSmallStM->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
	spriteSmallStM->addKeyframe(STAND_RIGHT, glm::vec2(0.125f, 0.f));
	spriteSmallStM->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

	spriteSmallStM->setAnimationSpeed(MOVE_LEFT, 16);
	spriteSmallStM->addKeyframe(MOVE_LEFT, glm::vec2(0.625f, 0.125f));
	spriteSmallStM->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.125f));
	spriteSmallStM->addKeyframe(MOVE_LEFT, glm::vec2(0.375f, 0.125f));

	spriteSmallStM->setAnimationSpeed(MOVE_RIGHT, 16);
	spriteSmallStM->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.125f));
	spriteSmallStM->addKeyframe(MOVE_RIGHT, glm::vec2(0.125f, 0.125f));
	spriteSmallStM->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.125f));

	spriteSmallStM->setAnimationSpeed(JUMP_LEFT, 16);
	spriteSmallStM->addKeyframe(JUMP_LEFT, glm::vec2(0.625f, 0.625f));
	spriteSmallStM->addKeyframe(JUMP_LEFT, glm::vec2(0.5f, 0.625f));
	spriteSmallStM->addKeyframe(JUMP_LEFT, glm::vec2(0.375f, 0.625f));

	spriteSmallStM->setAnimationSpeed(JUMP_RIGHT, 16);
	spriteSmallStM->addKeyframe(JUMP_RIGHT, glm::vec2(0.0f, 0.625f));
	spriteSmallStM->addKeyframe(JUMP_RIGHT, glm::vec2(0.125f, 0.625f));
	spriteSmallStM->addKeyframe(JUMP_RIGHT, glm::vec2(0.25f, 0.625f));

	spriteSmallStM->setAnimationSpeed(TURN_LEFT, 16);
	spriteSmallStM->addKeyframe(TURN_LEFT, glm::vec2(0.625f, 0.25f));
	spriteSmallStM->addKeyframe(TURN_LEFT, glm::vec2(0.5f, 0.25f));
	spriteSmallStM->addKeyframe(TURN_LEFT, glm::vec2(0.375f, 0.25f));

	spriteSmallStM->setAnimationSpeed(TURN_RIGHT, 16);
	spriteSmallStM->addKeyframe(TURN_RIGHT, glm::vec2(0.f, 0.25f));
	spriteSmallStM->addKeyframe(TURN_RIGHT, glm::vec2(0.125f, 0.25f));
	spriteSmallStM->addKeyframe(TURN_RIGHT, glm::vec2(0.25f, 0.25f));

	spriteSmallStM->setAnimationSpeed(STAND_FLAG, 16);
	spriteSmallStM->addKeyframe(STAND_FLAG, glm::vec2(0.0f, 0.375f));
	spriteSmallStM->addKeyframe(STAND_FLAG, glm::vec2(0.125f, 0.375f));
	spriteSmallStM->addKeyframe(STAND_FLAG, glm::vec2(0.25f, 0.375f));

	spriteSmallStM->setAnimationSpeed(RSTAND_FLAG, 16);
	spriteSmallStM->addKeyframe(RSTAND_FLAG, glm::vec2(0.625f, 0.375f));
	spriteSmallStM->addKeyframe(RSTAND_FLAG, glm::vec2(0.5f, 0.375f));
	spriteSmallStM->addKeyframe(RSTAND_FLAG, glm::vec2(0.375f, 0.375f));

	spriteSmallStM->setAnimationSpeed(DOWN_FLAG, 12);
	spriteSmallStM->addKeyframe(DOWN_FLAG, glm::vec2(0.0f, 0.375f));
	spriteSmallStM->addKeyframe(DOWN_FLAG, glm::vec2(0.125f, 0.375f));
	spriteSmallStM->addKeyframe(DOWN_FLAG, glm::vec2(0.25f, 0.375f));
	spriteSmallStM->addKeyframe(DOWN_FLAG, glm::vec2(0.0f, 0.5f));
	spriteSmallStM->addKeyframe(DOWN_FLAG, glm::vec2(0.125f, 0.5f));
	spriteSmallStM->addKeyframe(DOWN_FLAG, glm::vec2(0.25f, 0.5f));

	spritesheetSM.loadFromFile("images/superMario.png", TEXTURE_PIXEL_FORMAT_RGBA);
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
	tileMapDispl = tileMapPos;
	spriteM->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	oldPlayer = posPlayer;
	sprite = spriteM;	
}

void Player::flagTreatment()
{
	posPlayer.x = 197 * 32 + 18;
	if (sprite->animation() != DOWN_FLAG)
		sprite->changeAnimation(DOWN_FLAG);

	int maxDown = 11 * 32;
	if (spriteSize.y == 64)
		maxDown = 10 * 32;

	if (posPlayer.y < maxDown) {
		//sound
		if (!(engine->isCurrentlyPlaying("sounds/flagpole.wav"))) {
			engine->play2D("sounds/flagpole.wav", false, false, true);
		}

		posPlayer.y = posPlayer.y + 2;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}
	else {
		inFlag = false;
		bJumping = false;
		vx = 0.f;
		vy = 0.f;
		sprite->changeAnimation(RSTAND_FLAG);
		posPlayer.x = 198 * 32 + 11;
		posPlayer.y = maxDown;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		finalAnimation = true;
	}
}

void Player::getOut(int deltaTime) {
	countAnimation += deltaTime;
	if (countAnimation < 1000) {
	}
	else if (posPlayer.x < 199 * 32) {
		if (sprite->animation() != MOVE_RIGHT) {
			sprite->changeAnimation(MOVE_RIGHT);
		}

		posPlayer.x += 30;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}
	else if (posPlayer.y < (12 * 32) - 2) {
		if (sprite->animation() != JUMP_RIGHT) {
			posPlayer.x += 20;
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
			sprite->changeAnimation(JUMP_RIGHT);
		}
		else {
			posPlayer.x += 4;
			posPlayer.y += 2;
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
		}
	}
	else if (posPlayer.x < 200 * 32) {
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}
	int a = 1;
}

void Player::setMarioSprite() {
	superMario = false;
	starMario = false;
	int animation = sprite->animation();
	sprite = spriteM;
	sprite->changeAnimation(animation);

	if (spriteSize.y == 64) {
		posPlayer.y = posPlayer.y + 32;
		spriteSize = glm::ivec2(32, 32);
	}

	spriteSize = glm::ivec2(32, 32);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::setSuperMarioSprite() {
	if (!starMario && !superMario) {
		if (!(engine->isCurrentlyPlaying("sounds/getPowerup.wav"))) {
			engine->play2D("sounds/getPowerup.wav", false, false, true);
		}
	}

	int animation = sprite->animation();
	superMario = true;
	starMario = false;
	sprite = spriteSM;
	if (sprite->animation() != animation) {
		sprite->changeAnimation(animation);
	}
	if (spriteSize.y == 32) {
		posPlayer.y = posPlayer.y - 32;
		spriteSize = glm::ivec2(32, 64);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::setStarMarioSprite() {
	if (!starMario && !superMario) {
		if (!(engine->isCurrentlyPlaying("sounds/getPowerup.wav"))) {
			engine->play2D("sounds/getPowerup.wav", false, false, true);
		}
	}

	
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

void Player::update(int deltaTime)
{
	if (Game::instance().getKey('n') && (superMario || starMario))
	{
		setMarioSprite();
	}
	else if (Game::instance().getKey('m') && (!superMario || starMario))
	{
		setSuperMarioSprite();
	}
	else if (Game::instance().getKey('g') && !starMario)
	{
		setStarMarioSprite();
	}
	sprite->update(deltaTime);
	oldPlayer = posPlayer;

	if (inFlag && !finalAnimation) {
		flagTreatment();
	}
	else if (finalAnimation) {
		//getOut(deltaTime);
		posPlayer.x = 198 * 32 + 10;
	}
	else {
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (vx < -2.90f)
				sprite->setAnimationSpeed(MOVE_LEFT, 14);
			else
				sprite->setAnimationSpeed(MOVE_LEFT, 8);
			if (vx <= 0.f) {
				if (sprite->animation() != MOVE_LEFT && !bJumping && vx < 0.f)
					sprite->changeAnimation(MOVE_LEFT);
			}

			else {
				posPlayer.x += int(vx);
				if (!bJumping && vx > 0.f && sprite->animation() != TURN_LEFT)
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
			if (vx >= 0.f) {
				if (sprite->animation() != MOVE_RIGHT && !bJumping && vx > 0.f)
					sprite->changeAnimation(MOVE_RIGHT);
			}
			else {
				posPlayer.x += int(vx);
				if (!bJumping && vx < 0.f && sprite->animation() != TURN_RIGHT)
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
			if (map->collisionMoveRight(posPlayer, spriteSize, &posPlayer.x) && !bJumping) {
				if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) {
					vx = 0.0f;
					sprite->changeAnimation(STAND_LEFT);
				}
				else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT) {
					vx = 0.5f;
					sprite->changeAnimation(STAND_RIGHT);

				}
			}
		}

		else {
			if (map->collisionMoveLeft(posPlayer, spriteSize, &posPlayer.x, marg, ic) && !bJumping) {
				if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) {
					vx = -0.5f;
					sprite->changeAnimation(STAND_LEFT);
				}
				else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT) {
					vx = 0.0f;
					sprite->changeAnimation(STAND_RIGHT);

				}
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////
		
		if (small_jump) {
			if ((start_small_jump - posPlayer.y) >= 20) {
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
					// Jump Sound
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
						if (abs(startY - posPlayer.y) >= 128)
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
						bJumping = !map->collisionMoveUp(posPlayer, spriteSize, &posPlayer.y);
					else
						bJumping = !map->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y);
				}
				else {
					apex = true;
					if (abs(startY - posPlayer.y) >= 60)
						if (vy > -5.f)
							vy -= 0.08f * deltaTime;
					int dv = int(vy);
					posPlayer.y -= dv;
					if (vy > 0.f)
						bJumping = !map->collisionMoveUp(posPlayer, spriteSize, &posPlayer.y);
					else
						bJumping = !map->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y);
				}
			}
			else
			{
				apex = false;
				vy = 0.f;
				posPlayer.y += FALL_STEP;
				if (map->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y))
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
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}
	// MIRAR SI HACE FALTA
	// sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
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