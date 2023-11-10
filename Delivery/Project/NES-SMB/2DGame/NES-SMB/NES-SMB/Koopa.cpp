#include "Koopa.h"

#define FALL_STEP 4

enum KoopaAnims
{
	MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT
};

enum ShellKoopaAnims
{
	LIVE_SHELL, SHELL, DEATH_SHELL
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
	shell_hit_side = false;
	flipped = false;
	invinc = false;
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
	starMarioKill = false;
	first_hit = false;
	sprite_size = glm::ivec2(32, 64);
}

void Koopa::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spritesheetNKoopa, Texture &spritesheetCKoopa)
{
	shell_hit_side = false;
	flipped = false;
	invinc = false;
	vx = -1.f;
	spriteN = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.25f, 1.0f), &spritesheetNKoopa, &shaderProgram);
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

	spriteC = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5f, 0.5f), &spritesheetCKoopa, &shaderProgram);
	spriteC->setNumberAnimations(3);

	spriteC->setAnimationSpeed(LIVE_SHELL, 1);
	spriteC->addKeyframe(LIVE_SHELL, glm::vec2(0.5f, 0.0f));

	spriteC->setAnimationSpeed(SHELL, 1);
	spriteC->addKeyframe(SHELL, glm::vec2(0.0f, 0.0f));

	spriteC->setAnimationSpeed(DEATH_SHELL, 1);
	spriteC->addKeyframe(DEATH_SHELL, glm::vec2(0.0f, 0.5f));

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
	transitionState = false;
	hit = false;
	starMario = false;
	starMarioKill = false;
	first_hit = false;
	player_murderer = true;
	sprite_size = glm::ivec2(32, 64);
}


void Koopa::update(int deltaTime)
{
	sprite->update(deltaTime);
	oldEnemy = posEnemy;

	if (flipped) {
		starMarioKill = false;
		sprite->changeAnimation(DEATH_SHELL);
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
		if (!shield && (vx < 0.f)) {
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
		}
		else if (!shield && (vx > 0.f)) {
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
		}

		glm::ivec2 koopaColision = glm::ivec2(32, 64);
		if (shield) {
			koopaColision = glm::ivec2(32, 32);
		}

		posEnemy.x += int(vx);
		if (!invinc) {
			pair<int, float> state = map->collisionMarioEnemy(playerPos, marioSpriteSize, posEnemy, koopaColision);
			switch (state.first)
			{
			case 0:
			case 2:
			case 3:
				if (starMario) {
					starMarioKill = true;
					set_flipped_death();
				}
				else if (!dead) {
					if ((vx != 0.0f || vx != -0.0f) && !transitionState) {
						if (marioSpriteSize.y == 64) {
							hit = true;
						}
						else {
							dead_player = true;
						}
					}
					else if (shield && (vx == 0.0f || vx == -0.0f)) {
						if (state.first == 2) {
							shell_hit_side = true;
							transitionState = true;
							vx = 5.f;
						}
						else if (state.first == 3) {
							shell_hit_side = true;
							transitionState = true;
							vx = -5.f;
						}
					}
				}
				break;
			case 1:
				if (starMario) {
					set_flipped_death();
					starMarioKill = true;
				}
				else {
					if (shield) {
						if (vx != 0.f && !transitionState) {
							first_hit = true;
							transitionState = true;
							vx = 0.f;
						}
						else if (vx == 0.f && !transitionState) {
							if (state.second < 90.0f)
								vx = 5.f;
							else if (state.second >= 90.0f)
								vx = -5.f;

							first_hit = true;
							transitionState = true;
						}
					}
					else {
						vx = 0.f;
						transitionState = true;
						first_hit = true;
						shield = true;
						shell_sprite();
						posEnemy.y += 32;
					}
				}
				break;
			case -1:
				transitionState = false;
				break;
			}
		}

		posEnemy.y += FALL_STEP;
		map->collisionMoveDown(posEnemy, sprite_size, &posEnemy.y);

		if (map->collisionMoveRightEntity(posEnemy, sprite_size, &posEnemy.x))
			vx = -vx;
		if (map->collisionMoveLeftEntity(posEnemy, sprite_size, &posEnemy.x))
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

bool Koopa::get_shell_hit_side()
{
	return shell_hit_side;
}

void Koopa::unset_shell_hit_side()
{
	shell_hit_side = false;
}