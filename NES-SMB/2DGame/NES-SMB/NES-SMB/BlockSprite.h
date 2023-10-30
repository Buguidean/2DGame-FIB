#ifndef _BLOCK_SPRITE_INCLUDE
#define _BLOCK_SPRITE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Text is basically a Sprite that represents the texts.

class BlockSprite
{

public:
	virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	virtual void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::fvec2 getPosition();

	void obtainPosPlayer(const glm::ivec2 &pos);
	void setMarioSpriteSize(glm::ivec2 &size);

protected:
	glm::ivec2 marioSpriteSize, playerPos;
	glm::fvec2 tileMapDispl, posBlock;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _BLOCK_SPRITE_INCLUDE
