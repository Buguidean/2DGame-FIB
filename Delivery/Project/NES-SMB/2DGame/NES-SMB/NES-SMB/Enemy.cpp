#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Enemy.h"
#include "Game.h"


//#define JUMP_ANGLE_STEP 4
//#define JUMP_HEIGHT 100
//#define FALL_STEP 4

void Enemy::render()
{
	sprite->render();
}

void Enemy::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Enemy::setPosition(const glm::vec2 &pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

glm::ivec2 Enemy::getPosition()
{
	return posEnemy;
}

bool Enemy::killed()
{
	return dead;
}

bool Enemy::being_killed()
{
	return dying;
}

bool Enemy::playerKilled()
{
	return dead_player;
}

void Enemy::obtainPosPlayer(const glm::ivec2 &pos) 
{
	playerPos = pos;
}

void Enemy::setMarioSpriteSize(glm::ivec2 &size) {
	marioSpriteSize = size;
}

bool Enemy::isHit() {
	return hit;
}

void Enemy::setHit() {
	hit = false;
	marioSpriteSize.y = 32;
}
void Enemy::setStarMario(bool s) {
	starMario = s;
}

void Enemy::setInvMario(bool s) {
	invinc = s;
}

float* Enemy::getVelocity() {
	return &vx;
}

bool Enemy::get_player_murderer() {
	return player_murderer;
}

void Enemy::set_player_murderer(const bool &state) {
	player_murderer = state;
}

bool Enemy::get_starMarioKill() {
	return starMarioKill;
}
