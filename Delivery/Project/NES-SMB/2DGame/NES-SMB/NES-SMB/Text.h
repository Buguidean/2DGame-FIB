#ifndef _TEXT_INCLUDE
#define _TEXT_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Text is basically a Sprite that represents the texts.

class Text
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Texture &spritesheet);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setNumber(int num);
	glm::fvec2 getPosition();

private:
	glm::fvec2 tileMapDispl, posText;
	Sprite *sprite;
	TileMap *map;

};


#endif // _TEXT_INCLUDE
