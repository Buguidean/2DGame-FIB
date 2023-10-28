#include "Koopa.h"

enum KoopaAnims
{
	SHIELD, MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT
};


void Koopa::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	v = 1.f;
	spritesheet.loadFromFile("images/koopa.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(SHIELD, 4);
	sprite->addKeyframe(SHIELD, glm::vec2(0.25f, 0.5f));

	sprite->setAnimationSpeed(MOVE_LEFT, 4);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.0f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 4);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.0f));

	sprite->setAnimationSpeed(STAND_LEFT, 4);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.0f));

	sprite->setAnimationSpeed(STAND_RIGHT, 4);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.5f, 0.0f));

	sprite->changeAnimation(MOVE_LEFT);

	tileMapDispl = tileMapPos;
	int y = posEnemy.y + 32;
	int x = posEnemy.x;
	posEnemy = glm::ivec2(x, y);
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

}


void Koopa::update(int deltaTime)
{
	sprite->update(deltaTime);
	oldEnemy = posEnemy;

	if (dying) {
		//dying animation
		dead = true;
	}
	else if (shield) {
		if (sprite->animation() != SHIELD)
			sprite->changeAnimation(SHIELD);
	}
	else if (shieldState == 7) {
		dead = true;
	}
	else if (v < 0) {
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
	}
	else if (v > 0) {
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
	}

	int left = -1;
	glm::ivec2 koopaColision = glm::ivec2(32, 64);
	if (shield)
		koopaColision = glm::ivec2(32, 32);
	int state = map->collisionMarioEnemy(playerPos, marioSpriteSize, posEnemy, koopaColision, left);
	switch (state)
	{
	case 0:
		posEnemy.x -= int(v);
		if (starMario) {
			dying = true;
		}
		else {
			if (((!shield) || shieldState == 4) && (!dead || !dying)) {
				if (marioSpriteSize.y == 64 && (!dead || !dying)) {
					hit = true;
				}
				else if (!dead || !dying)
					dead_player = true;
			}
			else if (shield && shieldState == 2 && (!dead || !dying)) {
				if (left == 0)
					v = 1.f;
				else if (left == 1)
					v = -1.f;

				shieldState = 3;
				shieldCount = 0;
			}
		}
		break;
	case 1:
		v = 0.f;
		if (starMario)
			dying = true;
		else {
			if (shield) {
				if (transitionState && (shieldState == 2 || shieldState == 1)) {
					shieldState = 7;
					dying = true;
					v = 0.0f;
				}
				else if (shieldState == 4) {
					shieldState = 1;
				}
			}
			else {
				shield = true;
				shieldState = 1;

			}
			transitionState = false;
		}

		break;
	case -1:
		posEnemy.x -= int(v);
		transitionState = true;
		if (shield && (v == 0.f || v == -0.f)) {
			shieldState = 2;
		}
		if (shieldState == 3) {
			shieldState = 4;
		}
		break;
	}

	if (map->collisionMoveRight(posEnemy, glm::ivec2(32, 32), &posEnemy.x))
		v = -v;
	// El 2 es un "placeholder"
	if (map->collisionMoveLeft(posEnemy, glm::ivec2(32, 32), &posEnemy.x, false, 2))
		v = -v;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y - 32)));
}
