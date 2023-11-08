#include "BlockSprite.h"

void BlockSprite::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &animated_blocks) {}
void BlockSprite::update(int deltaTime) {}
bool BlockSprite::not_bumping() { return NULL; }
bool BlockSprite::check_colision() { return NULL; }
void BlockSprite::sprite_update(int deltaTime) {}
void BlockSprite::set_used() {}
void BlockSprite::set_inactive() {}
void BlockSprite::set_gift() {}
bool BlockSprite::get_gift() { return NULL; }
bool BlockSprite::is_question() { return NULL; }

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