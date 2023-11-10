#include "Goomba.h"


#define FALL_STEP 4

enum GoombaAnims
{
	GOOMBA_MOVE, GOOMBA_DEATH, GOOMBA_FLIPPED_DEATH
};

Goomba::~Goomba() 
{
	if (sprite != NULL)
		delete sprite;
}

void Goomba::reset() 
{
	flipped = false;
	jumped = false;
	counter = 0;
	vx = -1.f;
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	oldEnemy = posEnemy;
	playerPos = glm::ivec2(64, 352);
	dead = false;
	dead_player = false;
	invinc = false;
	hit = false;
	starMario = false;
	starMarioKill = false;
	dying = false;
	player_murderer = true;
}

void Goomba::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spritesheetGoomba)
{
	flipped = false;
	jumped = false;
	counter = 0;
	vx = -1.f;
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5f, 0.5f), &spritesheetGoomba, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(GOOMBA_FLIPPED_DEATH, 4);
	sprite->addKeyframe(GOOMBA_FLIPPED_DEATH, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(GOOMBA_DEATH, 4);
	sprite->addKeyframe(GOOMBA_DEATH, glm::vec2(0.0f, 0.5f));

	sprite->setAnimationSpeed(GOOMBA_MOVE, 4);
	sprite->addKeyframe(GOOMBA_MOVE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(GOOMBA_MOVE, glm::vec2(0.5, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	oldEnemy = posEnemy;
	playerPos = glm::ivec2(64, 352);
	invinc = false;
	dying = false;
	dead = false;
	dead_player = false;
	hit = false;
	starMario = false;
	starMarioKill = false;
	player_murderer = true;
}


void Goomba::update(int deltaTime)
{
	sprite->update(deltaTime);
	oldEnemy = posEnemy;

	if (flipped) {
		starMarioKill = false;
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

		if (!dying && sprite->animation() != GOOMBA_MOVE) {
			sprite->changeAnimation(GOOMBA_MOVE);
		}

		if (dying && counter < 500) {
			counter += deltaTime;
		}

		else if (dying) {
			//dying animation
			dead = true;
			dying = true;
			counter = 0;
		}

		posEnemy.x += int(vx);
		int state = -1;
		if (!invinc) {
			if (!dying) {
				state = map->collisionMarioEnemy(playerPos, marioSpriteSize, posEnemy, glm::ivec2(32, 32)).first;
			}

			switch (state)
			{
			case 0:
			case 2:
			case 3:
				posEnemy.x += int(vx);
				if (starMario) {
					set_flipped_death();
					starMarioKill = true;
					// vx = 0.f;
					// sprite->changeAnimation(GOOMBA_DEATH);
				}
				else if (marioSpriteSize.y == 64 && !dead) {
					hit = true;
					// marioSpriteSize = glm::ivec2(32, 32);
				}
				else if (!dead) {
					dead_player = true;
				}
				break;
			case 1:
				if (starMario) {
					set_flipped_death();
					starMarioKill = true;
				}
				else {
					vx = 0.f;
					sprite->changeAnimation(GOOMBA_DEATH);
					dying = true;
					jumped = true;
					posEnemy.y += 5;
				}
				break;
			case -1:
				break;
			}
		}

		if (!dying) {
			posEnemy.y += FALL_STEP;
			map->collisionMoveDown(posEnemy, glm::ivec2(32, 32), &posEnemy.y);

			if (map->collisionMoveRightEntity(posEnemy, glm::ivec2(32, 32), &posEnemy.x)) {
				vx = -vx;
			}
			if (map->collisionMoveLeftEntity(posEnemy, glm::ivec2(32, 32), &posEnemy.x)) {
				vx = -vx;
			}
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Goomba::setDying() {
	dying = true;
}

bool Goomba::get_jumped() {
	return jumped;
}

bool Goomba::get_flipped() {
	return flipped;
}

void Goomba::set_jumped(const bool state) {
	jumped = state;
}

void Goomba::set_flipped_death() {
	vy = 5.0f;
	start_jump = posEnemy.y;
	sprite->changeAnimation(GOOMBA_FLIPPED_DEATH);
	flipped = true;
}