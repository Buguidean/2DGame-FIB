#include "Koopa.h"

#define FALL_STEP 4

enum KoopaAnims
{
	LIVE_SHELL, SHELL, MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT, DEATH_SHELL
};

void Koopa::normal_sprite() {
	sprite = spriteN;
	sprite_size = glm::ivec2(32, 64);
}

void Koopa::shell_sprite() {
	sprite = spriteC;
	sprite_size = glm::ivec2(32, 32);
}

Koopa::~Koopa()
{
	if (spriteN != NULL) {
		delete spriteN;
	}
	if (spriteC != NULL) {
		delete spriteC;
	}
}

void Koopa::reset() 
{
	flipped = false;
	vx = -1.f;
	oldEnemy = posEnemy;
	playerPos = glm::ivec2(64, 352);

	shieldState = 0;
	shieldCount = 0;

	sprite = spriteN;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	player_murderer = true;
	shield = false;
	dying = false;
	dead = false;
	dead_player = false;
	transitionState = true;
	hit = false;
	starMario = false;
	first_hit = false;
	sprite_size = glm::ivec2(32, 64);
}

void Koopa::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	flipped = false;
	vx = -1.f;
	spritesheetN.loadFromFile("images/koopa.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteN = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.25f, 1.0f), &spritesheetN, &shaderProgram);
	spriteN->setNumberAnimations(4);

	spriteN->setAnimationSpeed(MOVE_LEFT, 4);
	spriteN->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.0f));
	spriteN->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.0f));

	spriteN->setAnimationSpeed(MOVE_RIGHT, 4);
	spriteN->addKeyframe(MOVE_RIGHT, glm::vec2(0.75f, 0.0f));
	spriteN->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.0f));
	
	spriteN->setAnimationSpeed(STAND_LEFT, 4);
	spriteN->addKeyframe(STAND_LEFT, glm::vec2(0.0f, 0.0f));

	spriteN->setAnimationSpeed(STAND_RIGHT, 4);
	spriteN->addKeyframe(STAND_RIGHT, glm::vec2(0.75f, 0.0f));

	spriteN->changeAnimation(MOVE_LEFT);

	spritesheetC.loadFromFile("images/koopa_shell.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteC = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5f, 0.5f), &spritesheetC, &shaderProgram);
	spriteC->setNumberAnimations(3);

	spriteC->setAnimationSpeed(SHELL, 1);
	spriteC->addKeyframe(SHELL, glm::vec2(0.0f, 0.0f));

	spriteC->setAnimationSpeed(DEATH_SHELL, 1);
	spriteC->addKeyframe(DEATH_SHELL, glm::vec2(0.0f, 0.5f));

	spriteC->setAnimationSpeed(LIVE_SHELL, 1);
	spriteC->addKeyframe(LIVE_SHELL, glm::vec2(0.5f, 0.0f));

	spriteC->changeAnimation(SHELL);

	sprite = spriteN;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));

	oldEnemy = posEnemy;
	playerPos = glm::ivec2(64, 352);

	shieldState = 0;
	shieldCount = 0;

	shield = false;
	dying = false;
	dead = false;
	dead_player = false;
	transitionState = true;
	hit = false;
	starMario = false;
	first_hit = false;
	player_murderer = true;
	sprite_size = glm::ivec2(32, 64);
}


void Koopa::update(int deltaTime)
{
	sprite->update(deltaTime);
	oldEnemy = posEnemy;
	
	if (flipped) {
		if ((start_jump - posEnemy.y) >= 20) {
			if (vy > -5.f) {
				vy -= 0.1f * deltaTime;
			}
		}
		int dvy = int(vy);
		int dvx = int(vx);
		posEnemy.y -= dvy;
		posEnemy.x += dvx;
		if (posEnemy.y > 600) {
			flipped = false;
			dead = true;
		}
	}

	else {

		if (dying) {
			//dying animation
			dead = true;
		}
		else if (shield) {
			// POR AHORA SE QUEDA ASI
		}
		else if (shieldState == 7) {
			dead = true;
		}
		else if (vx < 0) {
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
		}
		else if (vx > 0) {
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
		}

		int left = -1;
		glm::ivec2 koopaColision = glm::ivec2(32, 64);
		if (shield)
			koopaColision = glm::ivec2(32, 32);
		int state = map->collisionMarioEnemy(playerPos, marioSpriteSize, posEnemy, koopaColision);
		switch (state)
		{
		case 0:
		case 2:
		case 3:
			posEnemy.x += int(vx);
			if (starMario) {
				dying = true;
			}
			else {
				if (((!shield) || shieldState == 4) && (!dead || !dying)) {
					if (marioSpriteSize.y == 64 && (!dead || !dying)) {
						hit = true;
						// marioSpriteSize = glm::ivec2(32, 32);
					}
					else if (!dead || !dying)
						dead_player = true;
				}
				else if (shield && shieldState == 2 && (!dead || !dying)) {
					if (state == 2)
						vx = 5.f;
					else if (state == 3)
						vx = -5.f;

					shieldState = 3;
					shieldCount = 0;
				}
			}
			break;
		case 1:
			vx = 0.f;
			if (starMario)
				dying = true;
			else {
				if (shield) {
					if (transitionState && (shieldState == 2 || shieldState == 1)) {
						shieldState = 7;
						dying = true;
						vx = 0.0f;
					}
					else if (shieldState == 4) {
						shieldState = 1;
						first_hit = true;

					}
				}
				else {
					first_hit = true;
					shield = true;
					shell_sprite();
					posEnemy.y += 32;
					shieldState = 1;

				}
				transitionState = false;
			}

			break;
		case -1:
			posEnemy.x += int(vx);
			transitionState = true;
			if (shield && (vx == 0.f || vx == -0.f)) {
				shieldState = 2;
			}
			if (shieldState == 3) {
				shieldState = 4;
			}
			break;
		}

		posEnemy.y += FALL_STEP;
		map->collisionMoveDown(posEnemy, sprite_size, &posEnemy.y);

		if (map->collisionMoveRight(posEnemy, sprite_size, &posEnemy.x))
			vx = -vx;
		// El 2 es un "placeholder"
		if (map->collisionMoveLeft(posEnemy, sprite_size, &posEnemy.x, false, 2))
			vx = -vx;

	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

bool Koopa::hitted() {
	return first_hit;
}

void Koopa::disable_hitted() {
	first_hit = false;
}

void Koopa::setDying() {
	dying = true;
}

glm::ivec2 Koopa::get_sprite_size() {
	return sprite_size;
}

void Koopa::set_flipped_death() {
	vy = 5.0f;
	sprite = spriteC;
	sprite->changeAnimation(DEATH_SHELL);
	posEnemy.y += 32;
	start_jump = posEnemy.y;
	flipped = true;
}

bool Koopa::get_flipped() {
	return flipped;
}