#include "Goomba.h"

enum GoombaAnims
{
	GOOMBA_MOVE, GOOMBA_DEATH
};

void Goomba::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	counter = 0;
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
	playerPos = glm::ivec2(64, 352);
	dead = false;
	dead_player = false;
	dying = false;
}


void Goomba::update(int deltaTime)
{
	sprite->update(deltaTime);
	oldEnemy = posEnemy;

	if (dying && counter < 500) {
		counter += deltaTime;
	}
	else if (dying) {
		dead = true;
		dying = false;
		counter = 0;
	}

	if (!dying && sprite->animation() != GOOMBA_MOVE) {
		sprite->changeAnimation(GOOMBA_MOVE);
	}

	int state = -1;
	if (!dying) {
		state = map->collisionMarioEnemy(playerPos, glm::ivec2(32, 32), posEnemy, glm::ivec2(32, 32));
	}

	switch (state)
	{
	case 0:
		posEnemy.x -= int(v);
		dead_player = true;
		break;
	case 1:
		v = 0.f;
		sprite->changeAnimation(GOOMBA_DEATH);
		dying = true;
		posEnemy.y += 7;
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
