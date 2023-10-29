#include "Goomba.h"


#define FALL_STEP 4

enum GoombaAnims
{
	GOOMBA_MOVE, GOOMBA_DEATH
};

void Goomba::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

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
	hit = false;
	starMario = false;
}


void Goomba::update(int deltaTime)
{
	sprite->update(deltaTime);
	oldEnemy = posEnemy;
	if (sprite->animation() != GOOMBA_MOVE) {
		sprite->changeAnimation(GOOMBA_MOVE);
	}

	int state = map->collisionMarioEnemy(playerPos, marioSpriteSize, posEnemy, glm::ivec2(32, 32));

	switch (state)
	{
	case 0:
	case 2:
	case 3:
		posEnemy.x -= int(v);
		if (starMario) {
			dead = true;
			v = 0.f;
			sprite->changeAnimation(GOOMBA_DEATH);
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
		v = 0.f;
		sprite->changeAnimation(GOOMBA_DEATH);
		dead = true;
		break;
	case -1:
		posEnemy.x -= int(v);
		break;
	}

	posEnemy.y += FALL_STEP;
	map->collisionMoveDown(posEnemy, glm::ivec2(32, 32), &posEnemy.y);

	if (map->collisionMoveRight(posEnemy, glm::ivec2(32, 32), &posEnemy.x)) {
		v = -v;
	}
	// El 2 es un "placeholder"
	if (map->collisionMoveLeft(posEnemy, glm::ivec2(32, 32), &posEnemy.x, false, 2)) {
		v = -v;
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}
