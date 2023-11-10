#ifndef _BLOCK_SPRITE_INCLUDE
#define _BLOCK_SPRITE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Text is basically a Sprite that represents the texts.

class BlockSprite
{

public:
	virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &animated_blocks);
	virtual void update(int deltaTime);
	virtual bool not_bumping();
	virtual void sprite_update(int deltaTime);
	virtual bool check_colision();
	virtual void set_used();
	virtual void set_inactive();
	virtual void set_gift();
	virtual bool get_gift();
	virtual bool is_question();
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::fvec2 getPosition();

	void obtainPosPlayer(const glm::ivec2 &pos);
	void setMarioSpriteSize(glm::ivec2 &size);

protected:
	glm::ivec2 marioSpriteSize, playerPos;
	glm::fvec2 tileMapDispl, posBlock;
	Sprite *sprite;
	TileMap *map;

};


#endif // _BLOCK_SPRITE_INCLUDE
