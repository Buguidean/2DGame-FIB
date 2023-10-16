#include "Koopa.h"

enum KoopaAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Koopa::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	v = 1.f;
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

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	oldEnemy = posEnemy;
	playerPos = glm::ivec2(64, 352);
	dead = false;
	dead_player = false;
}


void Koopa::update(int deltaTime)
{
	sprite->update(deltaTime);
	oldEnemy = posEnemy;
	if (sprite->animation() != MOVE_LEFT) {
		sprite->changeAnimation(MOVE_LEFT);
	}

	int state = map->collisionMarioEnemy(playerPos, glm::ivec2(32, 32), posEnemy, glm::ivec2(32, 32));

	switch (state)
	{
	case 0:
		posEnemy.x -= int(v);
		dead_player = true;
		break;
	case 1:
		v = 0.f;
		sprite->changeAnimation(MOVE_LEFT);
		dead = true;
		break;
	case -1:
		posEnemy.x -= int(v);
		break;
	}

	if (map->collisionMoveRight(posEnemy, glm::ivec2(32, 32), &posEnemy.x))
		v = -v;
	// sprite->changeAnimation(STAND_RIGHT);
	// El 2 es un "placeholder"
	if (map->collisionMoveLeft(posEnemy, glm::ivec2(32, 32), &posEnemy.x, false, 2))
		v = -v;
	// sprite->changeAnimation(STAND_LEFT);


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}
