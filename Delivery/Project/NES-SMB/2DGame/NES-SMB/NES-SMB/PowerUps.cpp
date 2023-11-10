#include "PowerUps.h"


void PowerUps::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spritesheet) {}
void PowerUps::update(int deltaTime) {}
void PowerUps::set_render(const bool state) {}
void PowerUps::set_poping(const bool state) {}
bool PowerUps::get_render() { return NULL; }
int PowerUps::is_picked() { return NULL; }
bool PowerUps::is_coin() { return NULL; }

void PowerUps::render()
{
	sprite->render();
}

void PowerUps::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void PowerUps::setPosition(const glm::vec2 &pos)
{
	posPower = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPower.x), float(tileMapDispl.y + posPower.y)));
}

glm::fvec2 PowerUps::getPosition()
{
	return posPower;
}

void PowerUps::obtainPosPlayer(const glm::ivec2 &pos)
{
	playerPos = pos;
}

void PowerUps::setMarioSpriteSize(glm::ivec2 &size)
{
	marioSpriteSize = size;
}