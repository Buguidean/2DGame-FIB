#include "BlockSprite.h"

void BlockSprite::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {}
void BlockSprite::update(int deltaTime) {}
bool BlockSprite::not_bumping() { return NULL; }
void BlockSprite::sprite_update(int deltaTime) {}

void BlockSprite::render()
{
	sprite->render();
}

void BlockSprite::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void BlockSprite::setPosition(const glm::vec2 &pos)
{
	posBlock = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
}

glm::fvec2 BlockSprite::getPosition()
{
	return posBlock;
}

void BlockSprite::obtainPosPlayer(const glm::ivec2 &pos)
{
	playerPos = pos;
}

void BlockSprite::setMarioSpriteSize(glm::ivec2 &size) 
{
	marioSpriteSize = size;
}